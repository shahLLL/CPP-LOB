#pragma once

#include "types.hpp"
#include "pool_allocator.hpp"
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
    // Internal Structs, private by default, memory optimised using pragma pack.
#pragma pack(push, 1)
    struct LevelOrder final {
        ID orderID;
        Price orderPrice;
        Quantity currentQuantity;
        TimeStamp orderTimeStamp;
        LevelOrder* prev;
        LevelOrder* next;

        LevelOrder(ID orderID, Price orderPrice, Quantity orderQuantity,
            TimeStamp orderTimeStamp): orderID(orderID), 
            orderPrice(orderPrice), currentQuantity(orderQuantity),
            orderTimeStamp(orderTimeStamp), prev(nullptr), next(nullptr) {};
    };

    struct PriceLevel final {
        Quantity totalQuantity;
        LevelOrder* head = nullptr;
        LevelOrder* tail = nullptr;
    };

    struct Locator final {
        Price orderPrice;
        Quantity orderQuantity;
        Side orderSide;
        LevelOrder* node;
    };
#pragma pack(pop)

    // Internal Data Members, private by default.
    std::map<Price, PriceLevel, std::greater<Price>> bids;
    std::map<Price, PriceLevel, std::less<Price>> asks;
    std::unordered_map<ID, Locator> orderLocator;
    PoolAllocator<LevelOrder> orderPool;

    public:
        LOB() = default;
        Events submitOrder(const Order& order, const SizeT reserveCapacity = 4);
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