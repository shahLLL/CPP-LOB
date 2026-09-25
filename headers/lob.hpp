#pragma once

#include "types.hpp"
#include "pool_allocator.hpp"
#include <optional>
#include <cstdint>
#include <list>
#include <array>
#include <vector>
#include <unordered_map>

// Aliases
using SizeT = std::size_t;
using CursorType = std::int32_t;
using BitMapType = std::uint64_t;
using Double = double;
using Events = std::vector<Event>;
using Levels = std::vector<Level>;
using TOB = std::optional<Level>;

// Constant Expressions
static constexpr SizeT TICK_MAX = 20000; // Max value of security in cents.
static constexpr SizeT TICK_MIN = 5000; // Min value of security in cents.
static constexpr SizeT TICK_CAPACITY = TICK_MAX - TICK_MIN;
static constexpr SizeT BITMAP_CAPACITY = (TICK_CAPACITY % 64) == 0 ? 
    (TICK_CAPACITY/64) : (TICK_CAPACITY/64) + 1;
static constexpr CursorType NULL_CURSOR = -1;

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
    CursorType bestBidCursor = -1;
    CursorType bestAskCursor = -1;
    std::array<PriceLevel, TICK_CAPACITY> priceLadder;
    BitMapType bitMap[BITMAP_CAPACITY];

    std::unordered_map<ID, Locator> orderLocator;
    PoolAllocator<LevelOrder> orderPool;

    // Internal Helper Functions
    CursorType cursorSeekUp(const CursorType& inputCursor) const noexcept;
    CursorType cursorSeekDown(const CursorType& inputCursor) const noexcept;
    void removeFromBitMap(const CursorType& inputCursor) noexcept;
    void addToBitMap(const CursorType& inputCursor) noexcept;
    bool checkBitMap(const CursorType& inputCurosr) noexcept;

    public:
        LOB() = default;
        Events submitOrder(const Order& order, const SizeT& reserveCapacity = 4);
        Event cancelOrder(const ID& orderID, const TimeStamp& timeStamp);
        TOB getBestBid() const noexcept;
        TOB getBestAsk() const noexcept;
        Levels getBidOrderDepths(const SizeT& n) const;
        Levels getAskOrderDepths(const SizeT& n) const;
        Price getBidAskSpread() const;
        Price getMidPrice() const;
        Double getOrderImbalance() const;
        ~LOB() = default;
};