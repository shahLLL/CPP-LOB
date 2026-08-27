#include <catch2/catch_test_macros.hpp>
#include "../headers/lob.hpp"

TEST_CASE("BID ASK SPREAD TESTCASE #1", "[bid_ask_spread]") {
    // Check Bid Ask Spread for empty order book.
    LOB levelOrderBook = LOB();
    REQUIRE(levelOrderBook.getBidAskSpread() == 0.0);
}

TEST_CASE("BID ASK SPREAD TESTCASE #2", "[bid_ask_spread]") {
    // Check Bid Ask Spread for bids and no asks.
    LOB levelOrderBook = LOB();
    Price testPrice = 34.9;
    levelOrderBook.submitOrder(Order {
        1,
        testPrice,
        1,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC

    });
    REQUIRE(levelOrderBook.getBidAskSpread() == (testPrice * -1.0));
}

TEST_CASE("BID ASK SPREAD TESTCASE #3", "[bid_ask_spread]") {
    // Check Bid Ask Spread for asks and no bids.
    LOB levelOrderBook = LOB();
    Price testPrice = 34.9;
    levelOrderBook.submitOrder(Order {
        1,
        testPrice,
        1,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC

    });
    REQUIRE(levelOrderBook.getBidAskSpread() == testPrice);
}

TEST_CASE("BID ASK SPREAD TESTCASE #4", "[bid_ask_spread]") {
    // Check Bid Ask Spread for asks and no bids.
    LOB levelOrderBook = LOB();
    Price testPriceBid = 34.9;
    Price testPriceAsk = 54.7;
    levelOrderBook.submitOrder(Order {
        1,
        testPriceAsk,
        1,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC

    });
    levelOrderBook.submitOrder(Order {
        2,
        testPriceBid,
        1,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC

    });
    REQUIRE(levelOrderBook.getBidAskSpread() == (testPriceAsk - testPriceBid));
}