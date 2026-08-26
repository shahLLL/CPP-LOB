#include <catch2/catch_test_macros.hpp>
#include "../headers/lob.hpp"

TEST_CASE("BEST BID TEST CASE #1", "[get_best_bid]") {
    LOB levelOrderBook = LOB();

    Price testPrice1 = 12.5;
    Quantity testQuantity1 = 5;
    Price testPrice2 = testPrice1 / 2.0;
    Quantity testQuantity2 = testQuantity1 * 2;
    Price testPrice3 = testPrice1 * 2.0;
    Quantity testQuantity3 = testQuantity1 - 2;
    
    REQUIRE(!levelOrderBook.getBestBid().has_value());

    levelOrderBook.submitOrder(Order {
        1,
        testPrice1,
        testQuantity1,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(levelOrderBook.getBestBid().has_value());
    REQUIRE(levelOrderBook.getBestBid().value().price == testPrice1);
    REQUIRE(levelOrderBook.getBestBid().value().quantity == testQuantity1);

    levelOrderBook.submitOrder(Order {
        2,
        testPrice2,
        testQuantity2,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(levelOrderBook.getBestBid().has_value());
    REQUIRE(levelOrderBook.getBestBid().value().price == testPrice1);
    REQUIRE(levelOrderBook.getBestBid().value().quantity == testQuantity1);

    levelOrderBook.submitOrder(Order {
        3,
        testPrice3,
        testQuantity3,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(levelOrderBook.getBestBid().has_value());
    REQUIRE(levelOrderBook.getBestBid().value().price == testPrice1);
    REQUIRE(levelOrderBook.getBestBid().value().quantity == testQuantity1);

    levelOrderBook.submitOrder(Order {
        4,
        testPrice3,
        testQuantity3,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(levelOrderBook.getBestBid().has_value());
    REQUIRE(levelOrderBook.getBestBid().value().price == testPrice3);
    REQUIRE(levelOrderBook.getBestBid().value().quantity == testQuantity3);
}