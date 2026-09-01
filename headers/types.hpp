#pragma once

#include <cstdint>
#include <chrono>

// Type Aliases
using SizeT = std::size_t;
using ID = std::uint64_t;
using Quantity = std::int64_t;
using Price = double;
using EnumType = std::uint8_t;
using TimeStamp = std::chrono::system_clock::time_point;

// Enums
enum class Side : EnumType { BUY, SELL };
enum class OrderType : EnumType { LIMIT, MARKET };
enum class EventType : EnumType { REST, FILL, CANCEL, REJECT };
enum class RejectReason : EnumType { 
    DUPLICATE,
    UNKOWN,
    INVALID_PRICE,
    INVALID_QUANTITY, 
    ALREADY_TERMINAL,
    IOC_NO_FILL,
    MARKET_NO_FILL,
    FOK_INSUFFICIENT_LIQUIDITY,
    NOT_APPLICABLE
};
enum class CancelReason : EnumType {
    USER_REQUESTED,
    IOC_REMAINDER,
    MARKET_REMAINDER,
    DAY_EXPIRY,
    ENGINE_SHUTDOWN,
    NOT_APPLICABLE
};
/*
    Time-In-Force.
    GTC: Good Till Cancelled
    IOC: Immediate or Cancelled
    FOK: Fill or Cancel
    DAY: End of Session Cancel
    NON: No Time in Force Applicable
*/
enum class TimeInForce : EnumType { GTC, IOC, FOK, DAY, NON };

// Structs. Sorted from largest data type to smallest for memory optimisation.
struct Order {
    ID orderID;
    Price orderPrice;
    Quantity orderQuantity;
    TimeStamp orderTimeStamp;
    Side orderSide;
    OrderType orderType;
    TimeInForce orderTimeInForce;
};
struct Event {
    ID eventOrderID;
    ID counterOrderID;
    Price eventPrice;
    Quantity eventQuantity;
    TimeStamp eventTimeStamp;
    EventType eventType;
    RejectReason rejectReason;
    CancelReason cancelReason;
};
struct Level { Price price; Quantity quantity; };