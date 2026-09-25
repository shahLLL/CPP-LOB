#include "../headers/lob.hpp"

CursorType LOB::cursorSeekUp(const CursorType& inputCursor) const noexcept {
    CursorType cursor = inputCursor + 1;
    if (cursor < 0 || cursor >= TICK_CAPACITY) return NULL_CURSOR;
    CursorType idx = cursor >> 6;
    CursorType bit = cursor & 63;

    BitMapType mask = ~0ULL << bit;
    BitMapType word = bitMap[idx] & mask;

    while (word == 0) {
        if (++idx >= BITMAP_CAPACITY) return NULL_CURSOR;
        word = bitMap[idx];
    }

    CursorType trailingZeros = __builtin_ctzll(word);
    CursorType result = (idx << 6) + trailingZeros;
    if(result >= TICK_CAPACITY) return NULL_CURSOR;
    return result;
}

CursorType LOB::cursorSeekDown(const CursorType& inputCursor) const noexcept {
    CursorType cursor = inputCursor - 1;
    if (cursor < 0) return NULL_CURSOR;

    CursorType idx = cursor >> 6;
    CursorType bit = cursor & 63;
    BitMapType word = bitMap[idx] & ((bit == 63) ? ~0ULL : ((1ULL << (bit + 1)) - 1));

    while (word == 0) {
        if (--idx < 0) return NULL_CURSOR;
        word = bitMap[idx];
    }

    CursorType leadingZeros = __builtin_clzll(word);
    return (idx << 6) + (63 - leadingZeros);
}

void LOB::removeFromBitMap(const CursorType& inputCursor) noexcept { bitMap[inputCursor >> 6] &= ~(1ULL << (inputCursor & 63)); }

void LOB::addToBitMap(const CursorType& inputCursor) noexcept { bitMap[inputCursor >> 6] |= 1ULL << (inputCursor & 63); }

bool LOB::checkBitMap(const CursorType& inputCursor) noexcept { return ((bitMap[inputCursor >> 6] & (1ULL << (inputCursor & 63))) != 0); }

TOB LOB::getBestBid() const noexcept {
    if(bestBidCursor == NULL_CURSOR) return std::nullopt;
    return Level{ bestBidCursor + TICK_MIN, priceLadder[bestBidCursor].totalQuantity };
}

TOB LOB::getBestAsk() const noexcept {
    if(bestAskCursor == NULL_CURSOR) return std::nullopt;
    return Level{ bestAskCursor + TICK_MIN, priceLadder[bestAskCursor].totalQuantity };
}

Levels LOB::getBidOrderDepths(const SizeT& n) const {
    Levels bidDepths;
    bidDepths.reserve(n);
    CursorType cursor = bestBidCursor;
    while(cursor != NULL_CURSOR) {
        if(bidDepths.size() >= n) break;
        bidDepths.push_back(Level{ cursor + TICK_MIN, priceLadder[cursor].totalQuantity });
        cursor = cursorSeekDown(cursor);
    }

    return bidDepths;
}

Levels LOB::getAskOrderDepths(const SizeT& n) const {
    Levels askDepths;
    askDepths.reserve(n);
    CursorType cursor = bestAskCursor;
    while(cursor != NULL_CURSOR) {
        if(askDepths.size() >= n) break;
        askDepths.push_back(Level{ cursor + TICK_MIN, priceLadder[cursor].totalQuantity });
        cursor = cursorSeekUp(cursor);
    }
    return askDepths;
}

Price LOB::getBidAskSpread() const {
    if((bestBidCursor == NULL_CURSOR) && (bestAskCursor == NULL_CURSOR)) return 0;
    if((bestBidCursor == NULL_CURSOR) && (bestAskCursor != NULL_CURSOR)) return bestAskCursor + TICK_MIN;
    if((bestBidCursor != NULL_CURSOR) && (bestAskCursor == NULL_CURSOR)) return (bestBidCursor + TICK_MIN) * -1;
    return bestAskCursor - bestBidCursor;
}

Price LOB::getMidPrice() const {
    if((bestBidCursor == NULL_CURSOR) && (bestAskCursor == NULL_CURSOR)) return 0;
    if((bestBidCursor == NULL_CURSOR) && (bestAskCursor != NULL_CURSOR)) return bestAskCursor + TICK_MIN;
    if((bestBidCursor != NULL_CURSOR) && (bestAskCursor == NULL_CURSOR)) return bestBidCursor + TICK_MIN;
    return ((bestBidCursor + TICK_MIN) + (bestAskCursor + TICK_MIN)) / 2;
}

Double LOB::getOrderImbalance() const {
    Double bidTotal = 0.0;
    Double askTotal = 0.0;
    CursorType bidCursor = bestBidCursor;
    CursorType askCursor = bestAskCursor;

    while(bidCursor != NULL_CURSOR) {
        bidTotal += static_cast<Double>(priceLadder[bidCursor].totalQuantity);
        bidCursor = cursorSeekDown(bidCursor);
    }

    while(askCursor != NULL_CURSOR) {
        askTotal += static_cast<Double>(priceLadder[askCursor].totalQuantity);
        askCursor = cursorSeekUp(askCursor);
    }

    if(bidTotal + askTotal == 0.0) return 0.0;
    return (bidTotal - askTotal) / (bidTotal + askTotal);
}

Events LOB::submitOrder(const Order& order, const SizeT& reserveCapacity) {
    Events events;
    events.reserve(reserveCapacity);

    // Validation
    if(order.orderQuantity <= 0) {
        events.emplace_back(
            Event {
                order.orderID, 
                0, 
                order.orderPrice, 
                order.orderQuantity,
                order.orderTimeStamp,
                EventType::REJECT,
                RejectReason::INVALID_QUANTITY,
                CancelReason::NOT_APPLICABLE
            }
        );
        return events;
    }

    if((order.orderPrice >= TICK_MAX) || (order.orderPrice < TICK_MIN)) {
        events.emplace_back(
            Event {
                order.orderID,
                0,
                order.orderPrice,
                order.orderQuantity,
                order.orderTimeStamp,
                EventType::REJECT,
                RejectReason::INVALID_PRICE,
                CancelReason::NOT_APPLICABLE
            }
        );
        return events;
    }

    if(orderLocator.find(order.orderID) != orderLocator.end()) {
        events.emplace_back(
            Event {
                order.orderID,
                0,
                order.orderPrice,
                order.orderQuantity,
                order.orderTimeStamp,
                EventType::REJECT,
                RejectReason::DUPLICATE,
                CancelReason::NOT_APPLICABLE
            }
        );
        return events;
    }

    if(order.orderTimeInForce == TimeInForce::FOK) {
        Quantity requestedQuantity = order.orderQuantity;
        
        if(order.orderSide == Side::BUY) {
           CursorType askCursor = bestAskCursor;
           while(askCursor != NULL_CURSOR) {
                if(((askCursor + TICK_MIN) > order.orderPrice) || (requestedQuantity <= 0)) break;
                requestedQuantity = requestedQuantity - priceLadder[askCursor].totalQuantity;
                askCursor = cursorSeekUp(askCursor);
           }
        } else {
           CursorType bidCursor = bestBidCursor;
           while(bidCursor != NULL_CURSOR) {
                if(((bidCursor + TICK_MIN) < order.orderPrice) || (requestedQuantity <= 0)) break;
                requestedQuantity = requestedQuantity - priceLadder[bidCursor].totalQuantity;
                bidCursor = cursorSeekDown(bidCursor);
           }
        }

        if(requestedQuantity > 0) {
            events.emplace_back(
                Event {
                    order.orderID,
                    0,
                    order.orderPrice,
                    order.orderQuantity,
                    order.orderTimeStamp,
                    EventType::REJECT,
                    RejectReason::FOK_INSUFFICIENT_LIQUIDITY,
                    CancelReason::NOT_APPLICABLE
                }
            );
            return events;
        }
    }

    // Mathching
    Quantity requestedQuantity = order.orderQuantity;
    bool tradeExecuted = false;

    if(order.orderSide == Side::BUY) {
        CursorType askCursor = bestAskCursor;
        while((askCursor != NULL_CURSOR) && (requestedQuantity > 0)) {
            if((askCursor + TICK_MIN) > order.orderPrice) break;
            PriceLevel& priceLevel = priceLadder[askCursor];

            LevelOrder* current = priceLevel.head;
            while((current != nullptr) && (requestedQuantity > 0)) {
                Quantity fillQuantity = requestedQuantity < current->currentQuantity ? 
                    requestedQuantity : current->currentQuantity;
                
                requestedQuantity = requestedQuantity - fillQuantity;
                priceLevel.totalQuantity = priceLevel.totalQuantity - fillQuantity;
                current->currentQuantity = current->currentQuantity - fillQuantity;
                tradeExecuted = true;
                
                events.emplace_back(
                    Event {
                        order.orderID,
                        current->orderID,
                        askCursor + TICK_MIN,
                        fillQuantity,
                        order.orderTimeStamp,
                        EventType::FILL,
                        RejectReason::NOT_APPLICABLE,
                        CancelReason::NOT_APPLICABLE
                    }
                );

                LevelOrder* next = current->next;
                if(current->currentQuantity == 0) {
                    orderLocator.erase(current->orderID);

                    if (current->prev != nullptr) {
                        current->prev->next = current->next;
                    } else {
                        priceLevel.head = current->next;
                    }
                    if (current->next != nullptr) {
                        current->next->prev = current->prev;
                    } else {
                        priceLevel.tail = current->prev;
                    }

                    orderPool.destroy(current);
                }
                current = next;
            }

            if(priceLevel.totalQuantity == 0) {
                removeFromBitMap(askCursor);
                bestAskCursor = cursorSeekUp(askCursor);
                askCursor = bestAskCursor;
            }
            else break;
        }
    } else {
        CursorType bidCursor = bestBidCursor;
        while((bidCursor != NULL_CURSOR) && (requestedQuantity > 0)) {
            if((bidCursor + TICK_MIN) < order.orderPrice) break;
            PriceLevel& priceLevel = priceLadder[bidCursor];
            LevelOrder* current = priceLevel.head;

            while((current != nullptr) && (requestedQuantity > 0)) {
                Quantity fillQuantity = requestedQuantity < current->currentQuantity ?
                    requestedQuantity : current->currentQuantity;

                requestedQuantity = requestedQuantity - fillQuantity;
                priceLevel.totalQuantity = priceLevel.totalQuantity - fillQuantity;
                current->currentQuantity = current->currentQuantity - fillQuantity;
                tradeExecuted = true;

                events.emplace_back(
                    Event {
                        order.orderID,
                        current->orderID,
                        (bidCursor + TICK_MIN),
                        fillQuantity,
                        order.orderTimeStamp,
                        EventType::FILL,
                        RejectReason::NOT_APPLICABLE,
                        CancelReason::NOT_APPLICABLE
                    }
                );

                LevelOrder* next = current->next;
                if(current->currentQuantity == 0) {
                    orderLocator.erase(current->orderID);

                    if (current->prev != nullptr) {
                        current->prev->next = current->next;
                    } else {
                        priceLevel.head = current->next;
                    }
                    if (current->next != nullptr) {
                        current->next->prev = current->prev;
                    } else {
                        priceLevel.tail = current->prev;
                    }

                    orderPool.destroy(current);
                }
                current = next;
            }

            if(priceLevel.totalQuantity == 0) {
                removeFromBitMap(bidCursor);
                bestBidCursor = cursorSeekDown(bidCursor);
                bidCursor = bestBidCursor;
            }
            else break;
        }
    }

    if(requestedQuantity == 0) return events;

    // Handle Immediate Orders not valid for Rest.
    if((order.orderType == OrderType::MARKET) && (tradeExecuted)) {
        events.emplace_back(
            Event {
                order.orderID,
                0,
                order.orderPrice,
                requestedQuantity,
                order.orderTimeStamp,
                EventType::CANCEL,
                RejectReason::NOT_APPLICABLE,
                CancelReason::MARKET_REMAINDER
            }
        );
        return events;
    }

    if((order.orderType == OrderType::MARKET) && (!tradeExecuted)) {
        events.emplace_back(
            Event {
                order.orderID,
                0,
                order.orderPrice,
                order.orderQuantity,
                order.orderTimeStamp,
                EventType::REJECT,
                RejectReason::MARKET_NO_FILL,
                CancelReason::NOT_APPLICABLE
            }
        );
        return events;
    }

    if((order.orderTimeInForce == TimeInForce::IOC) && (tradeExecuted)) {
        events.emplace_back(
            Event {
                order.orderID,
                0,
                order.orderPrice,
                requestedQuantity,
                order.orderTimeStamp,
                EventType::CANCEL,
                RejectReason::NOT_APPLICABLE,
                CancelReason::IOC_REMAINDER
            }
        );
        return events;
    }

    if((order.orderTimeInForce == TimeInForce::IOC) && (!tradeExecuted)) {
        events.emplace_back(
            Event {
                order.orderID,
                0,
                order.orderPrice,
                order.orderQuantity,
                order.orderTimeStamp,
                EventType::REJECT,
                RejectReason::IOC_NO_FILL,
                CancelReason::NOT_APPLICABLE
            }
        );
        return events;
    }
    
    // Rest
    if(order.orderSide == Side::BUY) {
        CursorType bidCursor = order.orderPrice - TICK_MIN;
        bool bitMapCheck = checkBitMap(bidCursor);

        if(bitMapCheck) {
            PriceLevel& priceLevel = priceLadder[bidCursor];
            LevelOrder* node = orderPool.create(order.orderID, 
                order.orderPrice, requestedQuantity, order.orderTimeStamp);

            node->prev = priceLevel.tail;
            node->next = nullptr;
            priceLevel.tail->next = node;
            priceLevel.tail = node;
            priceLevel.totalQuantity = priceLevel.totalQuantity + requestedQuantity;

            orderLocator[order.orderID] = Locator { 
                order.orderPrice,
                requestedQuantity,
                Side::BUY,
                node
            };
        } else {
            PriceLevel& priceLevel = priceLadder[bidCursor];
            LevelOrder* node = orderPool.create(order.orderID, 
                order.orderPrice, requestedQuantity, order.orderTimeStamp);
            
            node->prev = nullptr;
            node->next = nullptr;
            priceLevel.head = node;
            priceLevel.tail = node;
            priceLevel.totalQuantity = requestedQuantity;

            orderLocator[order.orderID] = Locator {
                order.orderPrice,
                requestedQuantity,
                Side::BUY,
                node
            };
            addToBitMap(bidCursor);
            if(bidCursor > bestBidCursor) bestBidCursor = bidCursor;
        }
    } else {
        CursorType askCursor = order.orderPrice - TICK_MIN;
        if(checkBitMap(askCursor)) {
            PriceLevel& priceLevel = priceLadder[askCursor];
            LevelOrder* node = orderPool.create(order.orderID,
                order.orderPrice, requestedQuantity, order.orderTimeStamp);
            
            node->prev = priceLevel.tail;
            node->next = nullptr;
            priceLevel.tail->next = node;
            priceLevel.tail = node;
            priceLevel.totalQuantity = priceLevel.totalQuantity + requestedQuantity;

            orderLocator[order.orderID] = Locator { 
                order.orderPrice,
                requestedQuantity,
                Side::SELL,
                node
            };
        } else {
            PriceLevel& priceLevel = priceLadder[askCursor];
            LevelOrder* node = orderPool.create(order.orderID,
                order.orderPrice, requestedQuantity, order.orderTimeStamp);

            node->prev = nullptr;
            node->next = nullptr;
            priceLevel.head = node;
            priceLevel.tail = node;
            priceLevel.totalQuantity = requestedQuantity;

            orderLocator[order.orderID] = Locator {
                order.orderPrice,
                requestedQuantity,
                Side::SELL,
                node
            };
            addToBitMap(askCursor);
            if((bestAskCursor == NULL_CURSOR) || (askCursor < bestAskCursor)) bestAskCursor = askCursor;
        }
    }

    events.emplace_back(
        Event {
            order.orderID,
            0,
            order.orderPrice,
            requestedQuantity,
            order.orderTimeStamp,
            EventType::REST,
            RejectReason::NOT_APPLICABLE,
            CancelReason::NOT_APPLICABLE
        }
    );

    return events;
}

Event LOB::cancelOrder(const ID& orderID, const TimeStamp& timeStamp) {
    auto itr = orderLocator.find(orderID);
    if(itr == orderLocator.end()) {
        return Event {
            orderID,
            0,
            0,
            0,
            timeStamp,
            EventType::REJECT,
            RejectReason::UNKOWN,
            CancelReason::NOT_APPLICABLE
        };
    }

    const Locator& locatorRef = itr->second;
    const Price orderPrice = locatorRef.orderPrice;
    const Quantity orderQuantity = locatorRef.orderQuantity;
    LevelOrder* node = locatorRef.node;

    if(locatorRef.orderSide == Side::BUY) {
        CursorType bidCursor = locatorRef.orderPrice - TICK_MIN;
        PriceLevel& priceLevel = priceLadder[bidCursor];
        priceLevel.totalQuantity = 
            priceLevel.totalQuantity - node->currentQuantity;

        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else {
            priceLevel.head = node->next;
        }
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        } else {
            priceLevel.tail = node->prev;
        }

        if(priceLevel.head == nullptr) {
            if(bestBidCursor == bidCursor) bestBidCursor = cursorSeekDown(bidCursor);
            removeFromBitMap(bidCursor);
        }
        orderPool.destroy(node);
    } else {
        CursorType askCursor = locatorRef.orderPrice - TICK_MIN;
        PriceLevel& priceLevel = priceLadder[askCursor];
        priceLevel.totalQuantity = 
            priceLevel.totalQuantity - node->currentQuantity;
        
        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else {
            priceLevel.head = node->next;
        }
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        } else {
            priceLevel.tail = node->prev;
        }
        
        if(priceLevel.head == nullptr) {
            if(bestAskCursor == askCursor) bestAskCursor = cursorSeekUp(askCursor);
            removeFromBitMap(askCursor);
        } 
        orderPool.destroy(node);
    }
    orderLocator.erase(itr);

    return Event {
        orderID,
        0,
        orderPrice,
        orderQuantity,
        timeStamp,
        EventType::CANCEL,
        RejectReason::NOT_APPLICABLE,
        CancelReason::USER_REQUESTED
    };
}