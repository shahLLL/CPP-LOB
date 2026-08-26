#include <catch2/catch_test_macros.hpp>
#include "../headers/lob.hpp"

TEST_CASE("BEST ASK TEST CASE #1", "[get_best_ask]") {
    LOB levelOrderBook = LOB();

    Price testPrice1 = 124.51;
    Quantity testQuantity1 = 40;
    Price testPrice2 = testPrice1 * 2.0;
    Quantity testQuantity2 = testQuantity1 / 2;
    Price testPrice3 = testPrice1 / 2.0;
    Quantity testQuantity3 = testQuantity1 + 2;
    Price testPrice4 = testPrice1 / 1.5;
    Quantity testQuantity4 = testQuantity1 + 2;
    
    REQUIRE(!levelOrderBook.getBestAsk().has_value());

    levelOrderBook.submitOrder(Order {
        1,
        testPrice1,
        testQuantity1,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(levelOrderBook.getBestAsk().has_value());
    REQUIRE(levelOrderBook.getBestAsk().value().price == testPrice1);
    REQUIRE(levelOrderBook.getBestAsk().value().quantity == testQuantity1);

    levelOrderBook.submitOrder(Order {
        2,
        testPrice2,
        testQuantity2,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(levelOrderBook.getBestAsk().has_value());
    REQUIRE(levelOrderBook.getBestAsk().value().price == testPrice1);
    REQUIRE(levelOrderBook.getBestAsk().value().quantity == testQuantity1);

    levelOrderBook.submitOrder(Order {
        3,
        testPrice3,
        testQuantity3,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(levelOrderBook.getBestAsk().has_value());
    REQUIRE(levelOrderBook.getBestAsk().value().price == testPrice1);
    REQUIRE(levelOrderBook.getBestAsk().value().quantity == testQuantity1);

    levelOrderBook.submitOrder(Order {
        4,
        testPrice4,
        testQuantity4,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(levelOrderBook.getBestAsk().has_value());
    REQUIRE(levelOrderBook.getBestAsk().value().price == testPrice4);
    REQUIRE(levelOrderBook.getBestAsk().value().quantity == testQuantity4);
}