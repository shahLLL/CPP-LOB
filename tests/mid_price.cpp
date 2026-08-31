#include <catch2/catch_test_macros.hpp>
#include "../headers/lob.hpp"

TEST_CASE("BID ASK SPREAD TESTCASE #1", "[mid_price]") {
    // Check Mid Price for empty order book.
    LOB limitOrderBook = LOB();
    REQUIRE(limitOrderBook.getMidPrice() == 0.0);
}

TEST_CASE("BID ASK SPREAD TESTCASE #2", "[mid_price]") {
    // Check Mid Price for bids and no asks.
    LOB limitOrderBook = LOB();
    Price testPrice = 34.9;
    limitOrderBook.submitOrder(Order {
        1,
        testPrice,
        1,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC

    });
    REQUIRE(limitOrderBook.getMidPrice() == testPrice);
}

TEST_CASE("BID ASK SPREAD TESTCASE #3", "[mid_price]") {
    // Check Mid Price for asks and no bids.
    LOB limitOrderBook = LOB();
    Price testPrice = 34.9;
    limitOrderBook.submitOrder(Order {
        1,
        testPrice,
        1,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC

    });
    REQUIRE(limitOrderBook.getMidPrice() == testPrice);
}

TEST_CASE("BID ASK SPREAD TESTCASE #4", "[mid_price]") {
    // Check Mid Price for asks and bids.
    LOB limitOrderBook = LOB();
    Price testPriceBid = 34.0;
    Price testPriceAsk = 54.0;
    limitOrderBook.submitOrder(Order {
        1,
        testPriceAsk,
        1,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC

    });
    limitOrderBook.submitOrder(Order {
        2,
        testPriceBid,
        1,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC

    });
    REQUIRE(limitOrderBook.getMidPrice() == 44.0);
}