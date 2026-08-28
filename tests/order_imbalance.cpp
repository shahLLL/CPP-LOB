#include <catch2/catch_test_macros.hpp>
#include "../headers/lob.hpp"

TEST_CASE("ORDER IMBALANCE TEST CASE #1", "[get_order_imbalance]") {
    // Check order imbalance for an empty order book
    LOB limitOrderBook = LOB();
    REQUIRE(limitOrderBook.getOrderImbalance() == 0.0);
}

TEST_CASE("ORDER IMBALANCE TEST CASE #2", "[get_order_imbalance]") {
    // Check for a non-empty zero order imbalance.
    LOB limitOrderBook = LOB();
    limitOrderBook.submitOrder(Order {
        1,
        3.5,
        4,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        3.5,
        4,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        3.5,
        4,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        3.8,
        4,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        3.8,
        4,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        6,
        3.8,
        4,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(limitOrderBook.getOrderImbalance() == 0.0);
}

TEST_CASE("ORDER IMBALANCE TEST CASE #3", "[get_order_imbalance]") {
    // Check for a negative order imbalance.
    LOB limitOrderBook = LOB();
    limitOrderBook.submitOrder(Order {
        1,
        1.2,
        1,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        1.2,
        1,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        1.2,
        1,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        3.8,
        2,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        3.8,
        2,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        6,
        3.8,
        3,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(limitOrderBook.getOrderImbalance() == -0.4);
}

TEST_CASE("ORDER IMBALANCE TEST CASE #4", "[get_order_imbalance]") {
    // Check for a positive order imbalance.
    LOB limitOrderBook = LOB();
    limitOrderBook.submitOrder(Order {
        1,
        1.2,
        2,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        1.2,
        2,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        1.2,
        3,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        3.8,
        1,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        3.8,
        1,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        6,
        3.8,
        1,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(limitOrderBook.getOrderImbalance() == 0.4);
}