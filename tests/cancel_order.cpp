#include <catch2/catch_test_macros.hpp>
#include "../headers/lob.hpp"

TEST_CASE("CANCEL ORDER TESTCASE #1", "[cancel_order]") {
    // Checks to see if a cancel event for an absent order is rejected
    LOB limitOrderBook = LOB();
    ID orderID1 = 1;
    ID orderID2 = 2;
    Event cancelEvent1 = limitOrderBook.cancelOrder(orderID1);
    Event cancelEvent2 = limitOrderBook.cancelOrder(orderID2);

    REQUIRE(cancelEvent1.eventOrderID == orderID1);
    REQUIRE(cancelEvent1.counterOrderID == 0);
    REQUIRE(cancelEvent1.eventPrice == 0.0);
    REQUIRE(cancelEvent1.eventQuantity == 0);
    REQUIRE(cancelEvent1.eventType == EventType::REJECT);
    REQUIRE(cancelEvent1.rejectReason == RejectReason::UNKOWN);
    REQUIRE(cancelEvent1.cancelReason == CancelReason::NOT_APPLICABLE);

    REQUIRE(cancelEvent2.eventOrderID == orderID2);
    REQUIRE(cancelEvent2.counterOrderID == 0);
    REQUIRE(cancelEvent2.eventPrice == 0.0);
    REQUIRE(cancelEvent2.eventQuantity == 0);
    REQUIRE(cancelEvent2.eventType == EventType::REJECT);
    REQUIRE(cancelEvent2.rejectReason == RejectReason::UNKOWN);
    REQUIRE(cancelEvent2.cancelReason == CancelReason::NOT_APPLICABLE);
}

TEST_CASE("CANCEL ORDER TESTCASE #2", "[cancel_order]") {
    // Checks to see if orders that are present in the order book get successfully cancelled.
    LOB limitOrderBook = LOB();
    ID orderID1 = 1;
    ID orderID2 = 2;
    ID orderID3 = 3;
    Price price1 = 24.5;
    Price price2 = 224.5;
    Price price3 = 14.5;
    Quantity quantity1 = 3;
    Quantity quantity2 = 4;
    Quantity quantity3 = 10;

    limitOrderBook.submitOrder(Order {
        orderID1,
        price1,
        quantity1,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        orderID2,
        price2,
        quantity2,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        orderID3,
        price3,
        quantity3,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(limitOrderBook.getBidOrderDepths(2).size() == 2);
    REQUIRE(limitOrderBook.getAskOrderDepths(1).size() == 1);

    Event cancelEvent1 = limitOrderBook.cancelOrder(orderID1);
    Event cancelEvent2 = limitOrderBook.cancelOrder(orderID2);
    Event cancelEvent3 = limitOrderBook.cancelOrder(orderID3);

    REQUIRE(cancelEvent1.eventOrderID == orderID1);
    REQUIRE(cancelEvent1.counterOrderID == 0);
    REQUIRE(cancelEvent1.eventPrice == price1);
    REQUIRE(cancelEvent1.eventQuantity == quantity1);
    REQUIRE(cancelEvent1.eventType == EventType::CANCEL);
    REQUIRE(cancelEvent1.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(cancelEvent1.cancelReason == CancelReason::USER_REQUESTED);

    REQUIRE(cancelEvent2.eventOrderID == orderID2);
    REQUIRE(cancelEvent2.counterOrderID == 0);
    REQUIRE(cancelEvent2.eventPrice == price2);
    REQUIRE(cancelEvent2.eventQuantity == quantity2);
    REQUIRE(cancelEvent2.eventType == EventType::CANCEL);
    REQUIRE(cancelEvent2.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(cancelEvent2.cancelReason == CancelReason::USER_REQUESTED);

    REQUIRE(cancelEvent3.eventOrderID == orderID3);
    REQUIRE(cancelEvent3.counterOrderID == 0);
    REQUIRE(cancelEvent3.eventPrice == price3);
    REQUIRE(cancelEvent3.eventQuantity == quantity3);
    REQUIRE(cancelEvent3.eventType == EventType::CANCEL);
    REQUIRE(cancelEvent3.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(cancelEvent3.cancelReason == CancelReason::USER_REQUESTED);
}