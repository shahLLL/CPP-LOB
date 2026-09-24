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
        350,
        4,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        350,
        4,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        350,
        4,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        380,
        4,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        380,
        4,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        6,
        380,
        4,
        std::chrono::system_clock::now(),
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
        120,
        1,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        120,
        1,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        120,
        1,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        380,
        2,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        380,
        2,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        6,
        380,
        3,
        std::chrono::system_clock::now(),
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
        120,
        2,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        120,
        2,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        120,
        3,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        380,
        1,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        380,
        1,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        6,
        380,
        1,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(limitOrderBook.getOrderImbalance() == 0.4);
}