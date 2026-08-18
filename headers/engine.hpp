#pragma once

#include "types.hpp"
#include <optional>

// Aliases
using Events = std::vector<Event>;
using Levels = std::vector<Level>;
using TOB = std::optional<Level>;

class Engine final {
    Engine() = default;
    Events submitOrder(const Order& order);
    Events cancelOrder(UnsignedLong orderID);
    TOB getBestBid() const noexcept;
    TOB getBestAsk() const noexcept;
    Levels getBids() const;
    Levels getAsks() const;
    ~Engine() = default;
};
