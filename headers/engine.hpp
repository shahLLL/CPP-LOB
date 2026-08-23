#pragma once

#include "types.hpp"
#include <optional>
#include <list>
#include <map>

// Aliases
using SizeT = std::size_t;
using Double = double;
using Events = std::vector<Event>;
using Levels = std::vector<Level>;
using TOB = std::optional<Level>;

class Engine final {
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
        Side orderSide;
        Price orderPrice;
        std::list<LevelOrder>::iterator levelItr;
    };

    // Internal Data Members, private by default.
    std::map<Price, PriceLevel, std::greater<Price>> bids;
    std::map<Price, PriceLevel, std::less<Price>> asks;
    std::unordered_map<ID, Locator> orderLocator;

    public:
        Engine() = default;
        Events submitOrder(const Order& order);
        Events cancelOrder(ID orderID);
        TOB getBestBid() const noexcept;
        TOB getBestAsk() const noexcept;
        Levels getBidOrderDepths(SizeT n) const;
        Levels getAskOrderDepths(SizeT n) const;
        Price getBidAskSpread() const;
        Price getMidPrice() const;
        Double getOrderImbalance() const;
        ~Engine() = default;
};