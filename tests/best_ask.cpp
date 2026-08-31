#include <catch2/catch_test_macros.hpp>
#include "../headers/lob.hpp"

TEST_CASE("BEST ASK TEST CASE #1", "[get_best_ask]") {
    /*
    Check for best ask when:
    1. There are no asks.
    2. A new ask added
    3. An ask better than the best bid (in terms of price) is added
    4. An ask less than the best bid (in terms of price) is added
    */
    LOB limitOrderBook = LOB();

    Price testPrice1 = 124.51;
    Quantity testQuantity1 = 40;
    Price testPrice2 = testPrice1 * 2.0;
    Quantity testQuantity2 = testQuantity1 / 2;
    Price testPrice3 = testPrice1 / 2.0;
    Quantity testQuantity3 = testQuantity1 + 2;
    Price testPrice4 = testPrice1 / 1.5;
    Quantity testQuantity4 = testQuantity1 + 2;
    
    REQUIRE(!limitOrderBook.getBestAsk().has_value());

    limitOrderBook.submitOrder(Order {
        1,
        testPrice1,
        testQuantity1,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(limitOrderBook.getBestAsk().has_value());
    REQUIRE(limitOrderBook.getBestAsk().value().price == testPrice1);
    REQUIRE(limitOrderBook.getBestAsk().value().quantity == testQuantity1);

    limitOrderBook.submitOrder(Order {
        2,
        testPrice2,
        testQuantity2,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(limitOrderBook.getBestAsk().has_value());
    REQUIRE(limitOrderBook.getBestAsk().value().price == testPrice1);
    REQUIRE(limitOrderBook.getBestAsk().value().quantity == testQuantity1);

    limitOrderBook.submitOrder(Order {
        3,
        testPrice3,
        testQuantity3,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(limitOrderBook.getBestAsk().has_value());
    REQUIRE(limitOrderBook.getBestAsk().value().price == testPrice1);
    REQUIRE(limitOrderBook.getBestAsk().value().quantity == testQuantity1);

    limitOrderBook.submitOrder(Order {
        4,
        testPrice4,
        testQuantity4,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(limitOrderBook.getBestAsk().has_value());
    REQUIRE(limitOrderBook.getBestAsk().value().price == testPrice4);
    REQUIRE(limitOrderBook.getBestAsk().value().quantity == testQuantity4);
}

TEST_CASE("BEST ASK TEST CASE #2", "[get_best_ask]") {
    // Best Ask not effect by Buy Side orders
    LOB limitOrderBook = LOB();

    Price testPrice1 = 124.51;
    Quantity testQuantity1 = 40;
    Price testPrice2 = testPrice1 * 2.0;
    Quantity testQuantity2 = testQuantity1 / 2;
    Price testPrice3 = testPrice1 / 2.0;
    Quantity testQuantity3 = testQuantity1 + 2;
    Price testPrice4 = testPrice1 / 1.5;
    Quantity testQuantity4 = testQuantity1 + 2;
    
    REQUIRE(!limitOrderBook.getBestAsk().has_value());

    limitOrderBook.submitOrder(Order {
        1,
        testPrice1,
        testQuantity1,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(!limitOrderBook.getBestAsk().has_value());

    limitOrderBook.submitOrder(Order {
        2,
        testPrice2,
        testQuantity2,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(!limitOrderBook.getBestAsk().has_value());

    limitOrderBook.submitOrder(Order {
        3,
        testPrice3,
        testQuantity3,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(!limitOrderBook.getBestAsk().has_value());

    limitOrderBook.submitOrder(Order {
        4,
        testPrice4,
        testQuantity4,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(!limitOrderBook.getBestAsk().has_value());
}