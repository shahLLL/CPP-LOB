#include <catch2/catch_test_macros.hpp>
#include "../headers/lob.hpp"

TEST_CASE("SUBMIT ORDER TESTCASE #1", "[submit_order]") {
    // Checks for proper handling of invalid Order Quantities
    LOB limitOrderBook = LOB();

    Events events1 = limitOrderBook.submitOrder(Order {
        1,
        1250,
        0,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(events1.size() == 1);
    Event event1 = events1.at(0);
    REQUIRE(event1.eventOrderID == 1);
    REQUIRE(event1.counterOrderID == 0);
    REQUIRE(event1.eventPrice == 1250);
    REQUIRE(event1.eventQuantity == 0);
    REQUIRE(event1.eventType == EventType::REJECT);
    REQUIRE(event1.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event1.rejectReason == RejectReason::INVALID_QUANTITY);

    Events events2 = limitOrderBook.submitOrder(Order {
        1,
        1250,
        0,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(events2.size() == 1);
    Event event2 = events2.at(0);
    REQUIRE(event2.eventOrderID == 1);
    REQUIRE(event2.counterOrderID == 0);
    REQUIRE(event2.eventPrice == 1250);
    REQUIRE(event2.eventQuantity == 0);
    REQUIRE(event2.eventType == EventType::REJECT);
    REQUIRE(event2.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event2.rejectReason == RejectReason::INVALID_QUANTITY);

    Events events3 = limitOrderBook.submitOrder(Order {
        1,
        1250,
        -2,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(events3.size() == 1);
    Event event3 = events3.at(0);
    REQUIRE(event3.eventOrderID == 1);
    REQUIRE(event3.counterOrderID == 0);
    REQUIRE(event3.eventPrice == 1250);
    REQUIRE(event3.eventQuantity == -2);
    REQUIRE(event3.eventType == EventType::REJECT);
    REQUIRE(event3.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event3.rejectReason == RejectReason::INVALID_QUANTITY);

    Events events4 = limitOrderBook.submitOrder(Order {
        1,
        1250,
        0,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::MARKET,
        TimeInForce::GTC
    });
    REQUIRE(events4.size() == 1);
    Event event4 = events4.at(0);
    REQUIRE(event4.eventOrderID == 1);
    REQUIRE(event4.counterOrderID == 0);
    REQUIRE(event4.eventPrice == 1250);
    REQUIRE(event4.eventQuantity == 0);
    REQUIRE(event4.eventType == EventType::REJECT);
    REQUIRE(event4.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event4.rejectReason == RejectReason::INVALID_QUANTITY);

    Events events5 = limitOrderBook.submitOrder(Order {
        1,
        1250,
        0,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::FOK
    });
    REQUIRE(events5.size() == 1);
    Event event5 = events5.at(0);
    REQUIRE(event5.eventOrderID == 1);
    REQUIRE(event5.counterOrderID == 0);
    REQUIRE(event5.eventPrice == 1250);
    REQUIRE(event5.eventQuantity == 0);
    REQUIRE(event5.eventType == EventType::REJECT);
    REQUIRE(event5.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event5.rejectReason == RejectReason::INVALID_QUANTITY);

    Events events6 = limitOrderBook.submitOrder(Order {
        1,
        1250,
        0,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::IOC
    });
    REQUIRE(events6.size() == 1);
    Event event6 = events6.at(0);
    REQUIRE(event6.eventOrderID == 1);
    REQUIRE(event6.counterOrderID == 0);
    REQUIRE(event6.eventPrice == 1250);
    REQUIRE(event6.eventQuantity == 0);
    REQUIRE(event6.eventType == EventType::REJECT);
    REQUIRE(event6.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event6.rejectReason == RejectReason::INVALID_QUANTITY);

    Events events7 = limitOrderBook.submitOrder(Order {
        1,
        1250,
        0,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::DAY
    });
    REQUIRE(events7.size() == 1);
    Event event7 = events7.at(0);
    REQUIRE(event7.eventOrderID == 1);
    REQUIRE(event7.counterOrderID == 0);
    REQUIRE(event7.eventPrice == 1250);
    REQUIRE(event7.eventQuantity == 0);
    REQUIRE(event7.eventType == EventType::REJECT);
    REQUIRE(event7.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event7.rejectReason == RejectReason::INVALID_QUANTITY);
}

TEST_CASE("SUBMIT ORDER TESTCASE #2", "[submit_order]") {
    // Check to see if limit orders with a price of zero are rejected.
    LOB limitOrderBook = LOB();

    Events events1 = limitOrderBook.submitOrder(Order {
        1,
        0,
        2,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(events1.size() == 1);
    Event event1 = events1.at(0);
    REQUIRE(event1.eventOrderID == 1);
    REQUIRE(event1.counterOrderID == 0);
    REQUIRE(event1.eventPrice == 0);
    REQUIRE(event1.eventQuantity == 2);
    REQUIRE(event1.eventType == EventType::REJECT);
    REQUIRE(event1.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event1.rejectReason == RejectReason::INVALID_PRICE);

    Events events2 = limitOrderBook.submitOrder(Order {
        1,
        0,
        4,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(events2.size() == 1);
    Event event2 = events2.at(0);
    REQUIRE(event2.eventOrderID == 1);
    REQUIRE(event2.counterOrderID == 0);
    REQUIRE(event2.eventPrice == 0);
    REQUIRE(event2.eventQuantity == 4);
    REQUIRE(event2.eventType == EventType::REJECT);
    REQUIRE(event2.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event2.rejectReason == RejectReason::INVALID_PRICE);
}

TEST_CASE("SUBMIT ORDER TESTCASE #3", "[submit_order]") {
    // Validate duplicate orders get rejected.
    LOB limitOrderBook = LOB();

    limitOrderBook.submitOrder(Order {
        1,
        1250,
        3,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events1 = limitOrderBook.submitOrder(Order {
        1,
        1250,
        3,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(events1.size() == 1);
    Event event1 = events1.at(0);
    REQUIRE(event1.eventOrderID == 1);
    REQUIRE(event1.counterOrderID == 0);
    REQUIRE(event1.eventPrice == 1250);
    REQUIRE(event1.eventQuantity == 3);
    REQUIRE(event1.eventType == EventType::REJECT);
    REQUIRE(event1.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event1.rejectReason == RejectReason::DUPLICATE);

    limitOrderBook.submitOrder(Order {
        2,
        1250,
        3,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events2 = limitOrderBook.submitOrder(Order {
        2,
        950,
        4,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(events2.size() == 1);
    Event event2 = events2.at(0);
    REQUIRE(event2.eventOrderID == 2);
    REQUIRE(event2.counterOrderID == 0);
    REQUIRE(event2.eventPrice == 950);
    REQUIRE(event2.eventQuantity == 4);
    REQUIRE(event2.eventType == EventType::REJECT);
    REQUIRE(event2.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event2.rejectReason == RejectReason::DUPLICATE);
}

TEST_CASE("SUBMIT ORDER TESTCASE #4", "[submit_order]") { 
    // Check for rejection with FOK orders and not enough liquidity in the orderbook.
    LOB limitOrderBook1 = LOB();
    LOB limitOrderBook2 = LOB();
    LOB limitOrderBook3 = LOB();
    LOB limitOrderBook4 = LOB();
    LOB limitOrderBook5 = LOB();
    LOB limitOrderBook6 = LOB();

    Events events1 = limitOrderBook1.submitOrder(Order {
        1,
        1920,
        3,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::FOK
    });
    REQUIRE(events1.size() == 1);
    Event event1 = events1.at(0);
    REQUIRE(event1.eventOrderID == 1);
    REQUIRE(event1.counterOrderID == 0);
    REQUIRE(event1.eventPrice == 1920);
    REQUIRE(event1.eventQuantity == 3);
    REQUIRE(event1.eventType == EventType::REJECT);
    REQUIRE(event1.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event1.rejectReason == RejectReason::FOK_INSUFFICIENT_LIQUIDITY);

    Events events2 = limitOrderBook2.submitOrder(Order {
        2,
        1920,
        3,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::FOK
    });
    REQUIRE(events2.size() == 1);
    Event event2 = events2.at(0);
    REQUIRE(event2.eventOrderID == 2);
    REQUIRE(event2.counterOrderID == 0);
    REQUIRE(event2.eventPrice == 1920);
    REQUIRE(event2.eventQuantity == 3);
    REQUIRE(event2.eventType == EventType::REJECT);
    REQUIRE(event2.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event2.rejectReason == RejectReason::FOK_INSUFFICIENT_LIQUIDITY);

    limitOrderBook3.submitOrder(Order {
        1,
        4560,
        7,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events3 = limitOrderBook3.submitOrder(Order {
        2,
        1920,
        3,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::FOK
    });
    REQUIRE(events3.size() == 1);
    Event event3 = events3.at(0);
    REQUIRE(event3.eventOrderID == 2);
    REQUIRE(event3.counterOrderID == 0);
    REQUIRE(event3.eventPrice == 1920);
    REQUIRE(event3.eventQuantity == 3);
    REQUIRE(event3.eventType == EventType::REJECT);
    REQUIRE(event3.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event3.rejectReason == RejectReason::FOK_INSUFFICIENT_LIQUIDITY);

    limitOrderBook4.submitOrder(Order {
        1,
        1920,
        3,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events4 = limitOrderBook4.submitOrder(Order {
        2,
        4560,
        7,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::FOK
    });
    REQUIRE(events4.size() == 1);
    Event event4 = events4.at(0);
    REQUIRE(event4.eventOrderID == 2);
    REQUIRE(event4.counterOrderID == 0);
    REQUIRE(event4.eventPrice == 4560);
    REQUIRE(event4.eventQuantity == 7);
    REQUIRE(event4.eventType == EventType::REJECT);
    REQUIRE(event4.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event4.rejectReason == RejectReason::FOK_INSUFFICIENT_LIQUIDITY);

    limitOrderBook5.submitOrder(Order {
        1,
        3030,
        5,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events5 = limitOrderBook5.submitOrder(Order {
        2,
        3050,
        8,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::FOK
    });
    REQUIRE(events5.size() == 1);
    Event event5 = events5.at(0);
    REQUIRE(event5.eventOrderID == 2);
    REQUIRE(event5.counterOrderID == 0);
    REQUIRE(event5.eventPrice == 3050);
    REQUIRE(event5.eventQuantity == 8);
    REQUIRE(event5.eventType == EventType::REJECT);
    REQUIRE(event5.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event5.rejectReason == RejectReason::FOK_INSUFFICIENT_LIQUIDITY);

    limitOrderBook6.submitOrder(Order {
        1,
        3080,
        5,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events6 = limitOrderBook6.submitOrder(Order {
        2,
        3050,
        8,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::FOK
    });
    REQUIRE(events6.size() == 1);
    Event event6 = events6.at(0);
    REQUIRE(event6.eventOrderID == 2);
    REQUIRE(event6.counterOrderID == 0);
    REQUIRE(event6.eventPrice == 3050);
    REQUIRE(event6.eventQuantity == 8);
    REQUIRE(event6.eventType == EventType::REJECT);
    REQUIRE(event6.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event6.rejectReason == RejectReason::FOK_INSUFFICIENT_LIQUIDITY);
}

TEST_CASE("SUBMIT ORDER TESTCASE #5", "[submit_order]") {
    // Check for Bids being successfully matched.
    LOB levelOrderBook1 = LOB();
    LOB levelOrderBook2 = LOB();
    LOB levelOrderBook3 = LOB();
    LOB levelOrderBook4 = LOB();

    levelOrderBook1.submitOrder(Order {
        1,
        1340,
        5,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events1 = levelOrderBook1.submitOrder(Order {
        2,
        1340,
        5,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(events1.size() == 1);
    Event event1 = events1.at(0);
    REQUIRE(event1.eventOrderID == 2);
    REQUIRE(event1.counterOrderID == 1);
    REQUIRE(event1.eventPrice == 1340);
    REQUIRE(event1.eventQuantity == 5);
    REQUIRE(event1.eventType == EventType::FILL);
    REQUIRE(event1.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event1.rejectReason == RejectReason::NOT_APPLICABLE);

    levelOrderBook2.submitOrder(Order {
        1,
        1340,
        5,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events2 = levelOrderBook2.submitOrder(Order {
        2,
        1370,
        5,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(events2.size() == 1);
    Event event2 = events2.at(0);
    REQUIRE(event2.eventOrderID == 2);
    REQUIRE(event2.counterOrderID == 1);
    REQUIRE(event2.eventPrice == 1340);
    REQUIRE(event2.eventQuantity == 5);
    REQUIRE(event2.eventType == EventType::FILL);
    REQUIRE(event2.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event2.rejectReason == RejectReason::NOT_APPLICABLE);

    levelOrderBook3.submitOrder(Order {
        1,
        1340,
        5,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    levelOrderBook3.submitOrder(Order {
        2,
        1340,
        5,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events3 = levelOrderBook3.submitOrder(Order {
        3,
        1340,
        8,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(events3.size() == 2);

    Event event3A = events3.at(0);
    REQUIRE(event3A.eventOrderID == 3);
    REQUIRE(event3A.counterOrderID == 1);
    REQUIRE(event3A.eventPrice == 1340);
    REQUIRE(event3A.eventQuantity == 5);
    REQUIRE(event3A.eventType == EventType::FILL);
    REQUIRE(event3A.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event3A.rejectReason == RejectReason::NOT_APPLICABLE);
    Event event3B = events3.at(1);
    REQUIRE(event3B.eventOrderID == 3);
    REQUIRE(event3B.counterOrderID == 2);
    REQUIRE(event3B.eventPrice == 1340);
    REQUIRE(event3B.eventQuantity == 3);
    REQUIRE(event3B.eventType == EventType::FILL);
    REQUIRE(event3B.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event3B.rejectReason == RejectReason::NOT_APPLICABLE);

    levelOrderBook4.submitOrder(Order {
        1,
        1340,
        5,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    levelOrderBook4.submitOrder(Order {
        2,
        1360,
        5,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events4 = levelOrderBook4.submitOrder(Order {
        3,
        1380,
        8,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(events4.size() == 2);

    Event event4A = events4.at(0);
    REQUIRE(event4A.eventOrderID == 3);
    REQUIRE(event4A.counterOrderID == 1);
    REQUIRE(event4A.eventPrice == 1340);
    REQUIRE(event4A.eventQuantity == 5);
    REQUIRE(event4A.eventType == EventType::FILL);
    REQUIRE(event4A.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event4A.rejectReason == RejectReason::NOT_APPLICABLE);
    Event event4B = events4.at(1);
    REQUIRE(event4B.eventOrderID == 3);
    REQUIRE(event4B.counterOrderID == 2);
    REQUIRE(event4B.eventPrice == 1360);
    REQUIRE(event4B.eventQuantity == 3);
    REQUIRE(event4B.eventType == EventType::FILL);
    REQUIRE(event4B.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event4B.rejectReason == RejectReason::NOT_APPLICABLE);

}

TEST_CASE("SUBMIT ORDER TESTCASE #6", "[submit_order]") {
    // Check for Asks being successfully matched.
    LOB levelOrderBook1 = LOB();
    LOB levelOrderBook2 = LOB();
    LOB levelOrderBook3 = LOB();
    LOB levelOrderBook4 = LOB();

    levelOrderBook1.submitOrder(Order {
        1,
        1340,
        5,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events1 = levelOrderBook1.submitOrder(Order {
        2,
        1340,
        5,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(events1.size() == 1);
    Event event1 = events1.at(0);
    REQUIRE(event1.eventOrderID == 2);
    REQUIRE(event1.counterOrderID == 1);
    REQUIRE(event1.eventPrice == 1340);
    REQUIRE(event1.eventQuantity == 5);
    REQUIRE(event1.eventType == EventType::FILL);
    REQUIRE(event1.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event1.rejectReason == RejectReason::NOT_APPLICABLE);

    levelOrderBook2.submitOrder(Order {
        1,
        1340,
        5,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events2 = levelOrderBook2.submitOrder(Order {
        2,
        1230,
        5,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(events2.size() == 1);
    Event event2 = events2.at(0);
    REQUIRE(event2.eventOrderID == 2);
    REQUIRE(event2.counterOrderID == 1);
    REQUIRE(event2.eventPrice == 1340);
    REQUIRE(event2.eventQuantity == 5);
    REQUIRE(event2.eventType == EventType::FILL);
    REQUIRE(event2.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event2.rejectReason == RejectReason::NOT_APPLICABLE);

    levelOrderBook3.submitOrder(Order {
        1,
        1340,
        5,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    levelOrderBook3.submitOrder(Order {
        2,
        1340,
        5,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events3 = levelOrderBook3.submitOrder(Order {
        3,
        1340,
        8,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(events3.size() == 2);

    Event event3A = events3.at(0);
    REQUIRE(event3A.eventOrderID == 3);
    REQUIRE(event3A.counterOrderID == 1);
    REQUIRE(event3A.eventPrice == 1340);
    REQUIRE(event3A.eventQuantity == 5);
    REQUIRE(event3A.eventType == EventType::FILL);
    REQUIRE(event3A.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event3A.rejectReason == RejectReason::NOT_APPLICABLE);
    Event event3B = events3.at(1);
    REQUIRE(event3B.eventOrderID == 3);
    REQUIRE(event3B.counterOrderID == 2);
    REQUIRE(event3B.eventPrice == 1340);
    REQUIRE(event3B.eventQuantity == 3);
    REQUIRE(event3B.eventType == EventType::FILL);
    REQUIRE(event3B.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event3B.rejectReason == RejectReason::NOT_APPLICABLE);

    levelOrderBook4.submitOrder(Order {
        1,
        1340,
        5,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    levelOrderBook4.submitOrder(Order {
        2,
        1320,
        5,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events4 = levelOrderBook4.submitOrder(Order {
        3,
        1310,
        8,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(events4.size() == 2);

    Event event4A = events4.at(0);
    REQUIRE(event4A.eventOrderID == 3);
    REQUIRE(event4A.counterOrderID == 1);
    REQUIRE(event4A.eventPrice == 1340);
    REQUIRE(event4A.eventQuantity == 5);
    REQUIRE(event4A.eventType == EventType::FILL);
    REQUIRE(event4A.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event4A.rejectReason == RejectReason::NOT_APPLICABLE);
    Event event4B = events4.at(1);
    REQUIRE(event4B.eventOrderID == 3);
    REQUIRE(event4B.counterOrderID == 2);
    REQUIRE(event4B.eventPrice == 1320);
    REQUIRE(event4B.eventQuantity == 3);
    REQUIRE(event4B.eventType == EventType::FILL);
    REQUIRE(event4B.cancelReason == CancelReason::NOT_APPLICABLE);
    REQUIRE(event4B.rejectReason == RejectReason::NOT_APPLICABLE);
}

TEST_CASE("SUBMIT ORDER TESTCASE #7", "[submit_order]") { 
    // Check for FOK Orders successfully fulfilled
    LOB levelOrderBook1 = LOB();
    LOB levelOrderBook2 = LOB();

    levelOrderBook1.submitOrder(Order {
        1,
        1250,
        7,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events1 = levelOrderBook1.submitOrder(Order {
        2,
        1490,
        4,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::FOK
    });
    REQUIRE(events1.size() == 1);
    Event event1 = events1.at(0);
    REQUIRE(event1.eventOrderID == 2);
    REQUIRE(event1.counterOrderID == 1);
    REQUIRE(event1.eventPrice == 1250);
    REQUIRE(event1.eventQuantity == 4);
    REQUIRE(event1.eventType == EventType::FILL);
    REQUIRE(event1.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event1.cancelReason == CancelReason::NOT_APPLICABLE);

    levelOrderBook2.submitOrder(Order {
        1,
        1250,
        7,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events2 = levelOrderBook2.submitOrder(Order {
        2,
        1090,
        4,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::FOK
    });
    REQUIRE(events2.size() == 1);
    Event event2 = events2.at(0);
    REQUIRE(event2.eventOrderID == 2);
    REQUIRE(event2.counterOrderID == 1);
    REQUIRE(event2.eventPrice == 1250);
    REQUIRE(event2.eventQuantity == 4);
    REQUIRE(event2.eventType == EventType::FILL);
    REQUIRE(event2.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event2.cancelReason == CancelReason::NOT_APPLICABLE);
}

TEST_CASE("SUBMIT ORDER TESTCASE #8", "[submit_order]") { 
    // Check for Market Orders
    LOB limitOrderBook1 = LOB();
    LOB limitOrderBook2 = LOB();
    LOB limitOrderBook3 = LOB();

    limitOrderBook1.submitOrder(Order {
        1,
        3230,
        2,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events1 = limitOrderBook1.submitOrder(Order {
        2,
        3020,
        2,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::MARKET,
        TimeInForce::NON
    });
    REQUIRE(events1.size() == 1);
    Event event1 = events1.at(0);

    REQUIRE(event1.eventOrderID == 2);
    REQUIRE(event1.counterOrderID == 0);
    REQUIRE(event1.eventPrice == 3020);
    REQUIRE(event1.eventQuantity == 2);
    REQUIRE(event1.eventType == EventType::REJECT);
    REQUIRE(event1.rejectReason == RejectReason::MARKET_NO_FILL);
    REQUIRE(event1.cancelReason == CancelReason::NOT_APPLICABLE);

    limitOrderBook2.submitOrder(Order {
        1,
        3230,
        2,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events2 = limitOrderBook2.submitOrder(Order {
        2,
        3020,
        5,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::MARKET,
        TimeInForce::NON
    });
    REQUIRE(events2.size() == 2);

    Event event2A = events2.at(0);
    REQUIRE(event2A.eventOrderID == 2);
    REQUIRE(event2A.counterOrderID == 1);
    REQUIRE(event2A.eventPrice == 3230);
    REQUIRE(event2A.eventQuantity == 2);
    REQUIRE(event2A.eventType == EventType::FILL);
    REQUIRE(event2A.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event2A.cancelReason == CancelReason::NOT_APPLICABLE);

    Event event2B = events2.at(1);
    REQUIRE(event2B.eventOrderID == 2);
    REQUIRE(event2B.counterOrderID == 0);
    REQUIRE(event2B.eventPrice == 3020);
    REQUIRE(event2B.eventQuantity == 3);
    REQUIRE(event2B.eventType == EventType::CANCEL);
    REQUIRE(event2B.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event2B.cancelReason == CancelReason::MARKET_REMAINDER);

    limitOrderBook3.submitOrder(Order {
        1,
        3230,
        2,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events3 = limitOrderBook3.submitOrder(Order {
        2,
        3420,
        1,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::MARKET,
        TimeInForce::NON
    });
    REQUIRE(events3.size() == 1);
    Event event3 = events3.at(0);

    REQUIRE(event3.eventOrderID == 2);
    REQUIRE(event3.counterOrderID == 1);
    REQUIRE(event3.eventPrice == 3230);
    REQUIRE(event3.eventQuantity == 1);
    REQUIRE(event3.eventType == EventType::FILL);
    REQUIRE(event3.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event3.cancelReason == CancelReason::NOT_APPLICABLE);
}

TEST_CASE("SUBMIT ORDER TESTCASE #9", "[submit_order]") { 
    // Check for IOC Orders
    LOB limitOrderBook1 = LOB();
    LOB limitOrderBook2 = LOB();
    LOB limitOrderBook3 = LOB();

    limitOrderBook1.submitOrder(Order {
        1,
        3230,
        2,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events1 = limitOrderBook1.submitOrder(Order {
        2,
        3020,
        2,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::IOC
    });
    REQUIRE(events1.size() == 1);
    Event event1 = events1.at(0);

    REQUIRE(event1.eventOrderID == 2);
    REQUIRE(event1.counterOrderID == 0);
    REQUIRE(event1.eventPrice == 3020);
    REQUIRE(event1.eventQuantity == 2);
    REQUIRE(event1.eventType == EventType::REJECT);
    REQUIRE(event1.rejectReason == RejectReason::IOC_NO_FILL);
    REQUIRE(event1.cancelReason == CancelReason::NOT_APPLICABLE);

    limitOrderBook2.submitOrder(Order {
        1,
        3230,
        2,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events2 = limitOrderBook2.submitOrder(Order {
        2,
        3020,
        5,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::IOC
    });
    REQUIRE(events2.size() == 2);

    Event event2A = events2.at(0);
    REQUIRE(event2A.eventOrderID == 2);
    REQUIRE(event2A.counterOrderID == 1);
    REQUIRE(event2A.eventPrice == 3230);
    REQUIRE(event2A.eventQuantity == 2);
    REQUIRE(event2A.eventType == EventType::FILL);
    REQUIRE(event2A.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event2A.cancelReason == CancelReason::NOT_APPLICABLE);

    Event event2B = events2.at(1);
    REQUIRE(event2B.eventOrderID == 2);
    REQUIRE(event2B.counterOrderID == 0);
    REQUIRE(event2B.eventPrice == 3020);
    REQUIRE(event2B.eventQuantity == 3);
    REQUIRE(event2B.eventType == EventType::CANCEL);
    REQUIRE(event2B.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event2B.cancelReason == CancelReason::IOC_REMAINDER);

    limitOrderBook3.submitOrder(Order {
        1,
        3230,
        2,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events3 = limitOrderBook3.submitOrder(Order {
        2,
        3420,
        1,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::IOC
    });
    REQUIRE(events3.size() == 1);
    Event event3 = events3.at(0);

    REQUIRE(event3.eventOrderID == 2);
    REQUIRE(event3.counterOrderID == 1);
    REQUIRE(event3.eventPrice == 3230);
    REQUIRE(event3.eventQuantity == 1);
    REQUIRE(event3.eventType == EventType::FILL);
    REQUIRE(event3.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event3.cancelReason == CancelReason::NOT_APPLICABLE);
}

TEST_CASE("SUBMIT ORDER TESTCASE #10", "[submit_order]") {
    // Check for resting orders
    LOB limitOrderBook1 = LOB();
    LOB limitOrderBook2 = LOB();
    LOB limitOrderBook3 = LOB();
    LOB limitOrderBook4 = LOB();
    LOB limitOrderBook5 = LOB();
    LOB limitOrderBook6 = LOB();

    Events events1 = limitOrderBook1.submitOrder(Order {
        1,
        2890,
        3,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(events1.size() == 1);
    Event event1 = events1.at(0);
    REQUIRE(event1.eventOrderID == 1);
    REQUIRE(event1.counterOrderID == 0);
    REQUIRE(event1.eventPrice == 2890);
    REQUIRE(event1.eventQuantity == 3);
    REQUIRE(event1.eventType == EventType::REST);
    REQUIRE(event1.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event1.cancelReason == CancelReason::NOT_APPLICABLE);

    limitOrderBook2.submitOrder(Order {
        1,
        3570,
        5,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events2 = limitOrderBook2.submitOrder(Order {
        2,
        2890,
        3,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(events2.size() == 1);
    Event event2 = events2.at(0);
    REQUIRE(event2.eventOrderID == 2);
    REQUIRE(event2.counterOrderID == 0);
    REQUIRE(event2.eventPrice == 2890);
    REQUIRE(event2.eventQuantity == 3);
    REQUIRE(event2.eventType == EventType::REST);
    REQUIRE(event2.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event2.cancelReason == CancelReason::NOT_APPLICABLE);

    limitOrderBook3.submitOrder(Order {
        1,
        3570,
        5,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events3 = limitOrderBook3.submitOrder(Order {
        2,
        3890,
        9,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(events3.size() == 2);

    Event event3A = events3.at(0);
    REQUIRE(event3A.eventOrderID == 2);
    REQUIRE(event3A.counterOrderID == 1);
    REQUIRE(event3A.eventPrice == 3570);
    REQUIRE(event3A.eventQuantity == 5);
    REQUIRE(event3A.eventType == EventType::FILL);
    REQUIRE(event3A.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event3A.cancelReason == CancelReason::NOT_APPLICABLE);

    Event event3B = events3.at(1);
    REQUIRE(event3B.eventOrderID == 2);
    REQUIRE(event3B.counterOrderID == 0);
    REQUIRE(event3B.eventPrice == 3890);
    REQUIRE(event3B.eventQuantity == 4);
    REQUIRE(event3B.eventType == EventType::REST);
    REQUIRE(event3B.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event3B.cancelReason == CancelReason::NOT_APPLICABLE);

    Events events4 = limitOrderBook4.submitOrder(Order {
        1,
        2890,
        3,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(events4.size() == 1);
    Event event4 = events4.at(0);
    REQUIRE(event4.eventOrderID == 1);
    REQUIRE(event4.counterOrderID == 0);
    REQUIRE(event4.eventPrice == 2890);
    REQUIRE(event4.eventQuantity == 3);
    REQUIRE(event4.eventType == EventType::REST);
    REQUIRE(event4.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event4.cancelReason == CancelReason::NOT_APPLICABLE);

    limitOrderBook5.submitOrder(Order {
        1,
        3570,
        5,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events5 = limitOrderBook5.submitOrder(Order {
        2,
        3890,
        3,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(events5.size() == 1);
    Event event5 = events5.at(0);
    REQUIRE(event5.eventOrderID == 2);
    REQUIRE(event5.counterOrderID == 0);
    REQUIRE(event5.eventPrice == 3890);
    REQUIRE(event5.eventQuantity == 3);
    REQUIRE(event5.eventType == EventType::REST);
    REQUIRE(event5.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event5.cancelReason == CancelReason::NOT_APPLICABLE);

    limitOrderBook6.submitOrder(Order {
        1,
        3570,
        5,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    Events events6 = limitOrderBook6.submitOrder(Order {
        2,
        3390,
        9,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(events6.size() == 2);

    Event event6A = events6.at(0);
    REQUIRE(event6A.eventOrderID == 2);
    REQUIRE(event6A.counterOrderID == 1);
    REQUIRE(event6A.eventPrice == 3570);
    REQUIRE(event6A.eventQuantity == 5);
    REQUIRE(event6A.eventType == EventType::FILL);
    REQUIRE(event6A.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event6A.cancelReason == CancelReason::NOT_APPLICABLE);

    Event event6B = events6.at(1);
    REQUIRE(event6B.eventOrderID == 2);
    REQUIRE(event6B.counterOrderID == 0);
    REQUIRE(event6B.eventPrice == 3390);
    REQUIRE(event6B.eventQuantity == 4);
    REQUIRE(event6B.eventType == EventType::REST);
    REQUIRE(event6B.rejectReason == RejectReason::NOT_APPLICABLE);
    REQUIRE(event6B.cancelReason == CancelReason::NOT_APPLICABLE);
}