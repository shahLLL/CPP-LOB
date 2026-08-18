#pragma once

#include <cstdint>
#include <chrono>

// Type Aliases
using UnsignedChar = std::uint8_t;
using UnsignedLong = std::uint64_t;
using SignedLong = std::int64_t;
using TimeStamp = std::chrono::system_clock::time_point;

// Enums
enum class Side : UnsignedChar { BUY, SELL };
enum class OrderType : UnsignedChar { LIMIT, MARKET };
enum class EventType : UnsignedChar { ACCEPT, EXECUTE, CANCEL, REJECT };
enum class RejectReason : UnsignedChar { 
    DUPLICATE, 
    UNKOWN, 
    INVALID_PRICE, 
    INVALID_QUANTITY, 
    ALREADY_TERMINAL, 
    IOC_NO_FILL, 
    FOK_INSUFFICIENT_LIQUIDITY 
};
enum class CancelReason : UnsignedChar {
    USER_REQUESTED,
    IOC_REMAINDER,
    DAY_EXPIRY,
    ENGINE_SHUTDOWN
};
/*
    Timing Restrictions.
    GTC: Good Till Cancelled
    IOC: Immediate or Cancelled
    FOK: Fill or Cancel
    DAY: End of Session Cancel
*/
enum class TimeResc : UnsignedChar { GTC, IOC, FOK, DAY };

// Structs. Sorted from largest data type to smallest for memory optimisation.
struct Order {
    UnsignedLong orderID;
    SignedLong orderPrice;
    UnsignedLong orderQuantity;
    Side orderSide;
    OrderType orderType;
    TimeResc orderTimeResc;
    TimeStamp orderTimeStamp;
};
struct Event {
    UnsignedLong eventOrderID;
    UnsignedLong counterOrderID;
    SignedLong eventPrice;
    UnsignedLong eventQuantity;
    EventType eventType;
    TimeStamp eventTimeStamp;
    RejectReason rejectReason;
    CancelReason cancelReason;
};
struct Level { SignedLong price; UnsignedLong quantity; };