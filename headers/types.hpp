#pragma once

#include <cstdint>
#include <chrono>

// Type Aliases
using ID = std::uint64_t;
using Quantity = std::uint64_t;
using Price = double;
using EnumType = std::uint8_t;
using TimeStamp = std::chrono::system_clock::time_point;

// Enums
enum class Side : EnumType { BUY, SELL };
enum class OrderType : EnumType { LIMIT, MARKET };
enum class EventType : EnumType { ACCEPT, EXECUTE, CANCEL, REJECT };
enum class RejectReason : EnumType { 
    DUPLICATE, 
    UNKOWN, 
    INVALID_PRICE, 
    INVALID_QUANTITY, 
    ALREADY_TERMINAL, 
    IOC_NO_FILL, 
    FOK_INSUFFICIENT_LIQUIDITY 
};
enum class CancelReason : EnumType {
    USER_REQUESTED,
    IOC_REMAINDER,
    DAY_EXPIRY,
    ENGINE_SHUTDOWN
};
/*
    Time-In-Force.
    GTC: Good Till Cancelled
    IOC: Immediate or Cancelled
    FOK: Fill or Cancel
    DAY: End of Session Cancel
*/
enum class TimeInForce : EnumType { GTC, IOC, FOK, DAY };

// Structs. Sorted from largest data type to smallest for memory optimisation.
struct Order {
    ID orderID;
    Price orderPrice;
    Quantity orderQuantity;
    Side orderSide;
    OrderType orderType;
    TimeInForce orderTimeInForce;
    TimeStamp orderTimeStamp;
};
struct Event {
    ID eventOrderID;
    ID counterOrderID;
    Price eventPrice;
    Quantity eventQuantity;
    EventType eventType;
    TimeStamp eventTimeStamp;
    RejectReason rejectReason;
    CancelReason cancelReason;
};
struct Level { Price price; Quantity quantity; };