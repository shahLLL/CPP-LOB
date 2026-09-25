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
        8500,
        4,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        8500,
        4,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        8500,
        4,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        8800,
        4,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        8800,
        4,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        6,
        8800,
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
        5200,
        1,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        5200,
        1,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        5200,
        1,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        8800,
        2,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        8800,
        2,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        6,
        8800,
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
        5200,
        2,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        5200,
        2,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        5200,
        3,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        8800,
        1,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        8800,
        1,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        6,
        8800,
        1,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(limitOrderBook.getOrderImbalance() == 0.4);
}