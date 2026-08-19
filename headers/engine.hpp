#pragma once

#include "types.hpp"
#include <optional>
#include <list>
#include <map>

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

    // Internal Data Members, private by default.
    std::map<SignedLong, PriceLevel, std::greater<SignedLong>> bids;
    std::map<SignedLong, PriceLevel, std::less<SignedLong>> asks;
    std::unordered_map<UnsignedLong, Locator> orderLocator;

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