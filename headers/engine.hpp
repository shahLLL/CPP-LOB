#pragma once

#include "types.hpp"
#include <optional>
#include <list>

// Aliases
using Events = std::vector<Event>;
using Levels = std::vector<Level>;
using TOB = std::optional<Level>;

class Engine final {
    // Internal Types, private by default.
    struct LevelOrder {
        UnsignedLong orderID;
        UnsignedLong orderPrice;
        UnsignedLong orderQuantity;
        TimeStamp orderTimeStamp;
    };
    struct PriceLevel {
        std::list<LevelOrder> level;
        UnsignedLong totalQuantity;
    };
    struct Locator {
        Side orderSide;
        SignedLong orderPrice;
        std::list<LevelOrder>::iterator levelItr;
    };

    public:
        Engine() = default;
        Events submitOrder(const Order& order);
        Events cancelOrder(UnsignedLong orderID);
        TOB getBestBid() const noexcept;
        TOB getBestAsk() const noexcept;
        Levels getBids(UnsignedLong n) const;
        Levels getAsks(UnsignedLong n) const;
        ~Engine() = default;
};