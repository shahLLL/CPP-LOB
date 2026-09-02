#include "../headers/lob.hpp"

TOB LOB::getBestBid() const noexcept {
    auto bidsItr = bids.begin();
    if(bids.empty()) return std::nullopt;
    return Level{ bidsItr->first, bidsItr->second.totalQuantity };
}

TOB LOB::getBestAsk() const noexcept {
    auto asksItr = asks.begin();
    if(asks.empty()) return std::nullopt;
    return Level{ asksItr->first, asksItr->second.totalQuantity };
}

Levels LOB::getBidOrderDepths(SizeT n) const {
    Levels bidDepths;
    bidDepths.reserve(n);
    for(auto& [price, priceLevel] : bids) {
        if(bidDepths.size() >= n) break;
        bidDepths.push_back(Level{ price, priceLevel.totalQuantity });
    }
    return bidDepths;
}

Levels LOB::getAskOrderDepths(SizeT n) const {
    Levels askDepths;
    askDepths.reserve(n);
    for(auto& [price, priceLevel] : asks) {
        if(askDepths.size() >= n) break;
        askDepths.push_back(Level{ price, priceLevel.totalQuantity });
    }
    return askDepths;
}

Price LOB::getBidAskSpread() const {
    if(bids.empty() && asks.empty()) return 0.0;
    if(bids.empty() && !asks.empty()) return asks.begin()->first;
    if(!bids.empty() && asks.empty()) return (bids.begin()->first) * -1.0;
    return (asks.begin()->first) - (bids.begin()->first);
}

Price LOB::getMidPrice() const {
    if(bids.empty() && asks.empty()) return 0.0;
    if(bids.empty() && !asks.empty()) return asks.begin()->first;
    if(!bids.empty() && asks.empty()) return bids.begin()->first;
    return (((bids.begin()->first) + (asks.begin()->first)) / 2.0);
}

Double LOB::getOrderImbalance() const {
    Double bidTotal = 0.0;
    Double askTotal = 0.0;
    for(auto& [price, priceLevel] : bids) { 
        bidTotal += static_cast<Double>(priceLevel.totalQuantity);
    }
    for(auto& [price, priceLevel] : asks) { 
        askTotal += static_cast<Double>(priceLevel.totalQuantity);
    }
    if(bidTotal + askTotal == 0.0) return 0.0;
    return (bidTotal - askTotal) / (bidTotal + askTotal);
}

Events LOB::submitOrder(const Order& order, const SizeT reserveCapacity) {
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

    if((order.orderType == OrderType::LIMIT) && (order.orderPrice <= 0.0)) {
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
            for(auto& [price, priceLevel] : asks) {
                if((price > order.orderPrice) || (requestedQuantity <= 0)) break;
                requestedQuantity = requestedQuantity - priceLevel.totalQuantity;
            }
        } else {
            for(auto& [price, priceLevel] : bids) {
                if((price < order.orderPrice) || (requestedQuantity <= 0)) break;
                requestedQuantity = requestedQuantity - priceLevel.totalQuantity;
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
        auto asksItr = asks.begin();
        while((asksItr != asks.end()) && (requestedQuantity > 0)) {
            if(asksItr->first > order.orderPrice) break;

            auto& priceLevel = asksItr->second;
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
                        asksItr->first,
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

            if(priceLevel.totalQuantity == 0) asksItr = asks.erase(asksItr);
            else break;
        }
    } else {
        auto bidsItr = bids.begin();
        while((bidsItr != bids.end()) && (requestedQuantity > 0)) {
            if(bidsItr->first < order.orderPrice) break;

            auto& priceLevel = bidsItr->second;
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
                        bidsItr->first,
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

            if(priceLevel.totalQuantity == 0) bidsItr = bids.erase(bidsItr);
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
        auto bidsItr = bids.find(order.orderPrice);
        if(bidsItr != bids.end()) {
            auto& priceLevel = bids[order.orderPrice];
            LevelOrder* node = orderPool.create(order.orderID, 
                order.orderPrice, requestedQuantity, order.orderTimeStamp);

            node->prev = priceLevel.tail;
            node->next = nullptr;
            if (priceLevel.tail != nullptr) {
                priceLevel.tail->next = node;
            } else {
                priceLevel.head = node;
            }
            priceLevel.tail = node;

            priceLevel.totalQuantity = priceLevel.totalQuantity + requestedQuantity;
            orderLocator[order.orderID] = Locator { 
                order.orderPrice,
                requestedQuantity,
                Side::BUY,
                node
            };
        } else {
            auto [it, inserted] = bids.emplace(
                order.orderPrice,
                PriceLevel{}
            );
            auto& priceLevel = it->second;
            LevelOrder* node = orderPool.create(order.orderID, 
                order.orderPrice, requestedQuantity, order.orderTimeStamp);

            
            node->prev = priceLevel.tail;
            node->next = nullptr;
            if (priceLevel.tail != nullptr) {
                priceLevel.tail->next = node;
            } else {
               priceLevel.head = node;
            }
            priceLevel.tail = node;

            priceLevel.totalQuantity = requestedQuantity;
            orderLocator[order.orderID] = Locator {
                order.orderPrice,
                requestedQuantity,
                Side::BUY,
                node
            };
        }
    } else {
        auto asksItr = asks.find(order.orderPrice);
        if(asksItr != asks.end()) {
            auto& priceLevel = asks[order.orderPrice];
            LevelOrder* node = orderPool.create(order.orderID,
                order.orderPrice, requestedQuantity, order.orderTimeStamp);
            
            node->prev = priceLevel.tail;
            node->next = nullptr;
            if (priceLevel.tail != nullptr) {
                priceLevel.tail->next = node;
            } else {
                priceLevel.head = node;
            }
            priceLevel.tail = node;

            priceLevel.totalQuantity = priceLevel.totalQuantity + requestedQuantity;
            orderLocator[order.orderID] = Locator { 
                order.orderPrice,
                requestedQuantity,
                Side::SELL,
                node
            };
        } else {
            auto [it, inserted] = asks.emplace(
                order.orderPrice,
                PriceLevel{}
            );
            auto& priceLevel = it->second;
            LevelOrder* node = orderPool.create(order.orderID, 
                order.orderPrice, requestedQuantity, order.orderTimeStamp);

            node->prev = priceLevel.tail;
            node->next = nullptr;
            if (priceLevel.tail != nullptr) {
                priceLevel.tail->next = node;
            } else {
                priceLevel.head = node;
            }
            priceLevel.tail = node;

            priceLevel.totalQuantity = requestedQuantity;
            orderLocator[order.orderID] = Locator {
                order.orderPrice,
                requestedQuantity,
                Side::SELL,
                node
            };
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

Event LOB::cancelOrder(ID orderID, TimeStamp ts) {
    auto itr = orderLocator.find(orderID);
    if(itr == orderLocator.end()) {
        return Event {
            orderID,
            0,
            0.0,
            0,
            ts,
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
        auto mapItr = bids.find(locatorRef.orderPrice);
        auto& priceLevel = mapItr->second;
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

        orderPool.destroy(node);
        if(priceLevel.head == nullptr) bids.erase(mapItr);
    } else {
        auto mapItr = asks.find(locatorRef.orderPrice);
        auto& priceLevel = mapItr->second;
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

        orderPool.destroy(node);
        if(priceLevel.head == nullptr) asks.erase(mapItr);
    }
    orderLocator.erase(itr);

    return Event {
        orderID,
        0,
        orderPrice,
        orderQuantity,
        ts,
        EventType::CANCEL,
        RejectReason::NOT_APPLICABLE,
        CancelReason::USER_REQUESTED
    };
}