#pragma once

#include "types.hpp"
#include <optional>
#include <list>
#include <map>
#include <vector>
#include <unordered_map>

// Aliases
using SizeT = std::size_t;
using Double = double;
using Events = std::vector<Event>;
using Levels = std::vector<Level>;
using TOB = std::optional<Level>;

class LOB final {
    // Internal Types, private by default.
    struct LevelOrder {
        ID orderID;
        Price orderPrice;
        Quantity currentQuantity;
        TimeStamp orderTimeStamp;
    };
    struct PriceLevel {
        std::list<LevelOrder> level;
        Quantity totalQuantity;
    };
    struct Locator {
        std::list<LevelOrder>::iterator levelItr;
        Price orderPrice;
        Quantity orderQuantity;
        Side orderSide;
    };

    // Internal Data Members, private by default.
    std::map<Price, PriceLevel, std::greater<Price>> bids;
    std::map<Price, PriceLevel, std::less<Price>> asks;
    std::unordered_map<ID, Locator> orderLocator;

    public:
        LOB() = default;
        Events submitOrder(const Order& order);
        Event cancelOrder(ID orderID, TimeStamp ts);
        TOB getBestBid() const noexcept;
        TOB getBestAsk() const noexcept;
        Levels getBidOrderDepths(SizeT n) const;
        Levels getAskOrderDepths(SizeT n) const;
        Price getBidAskSpread() const;
        Price getMidPrice() const;
        Double getOrderImbalance() const;
        ~LOB() = default;
};