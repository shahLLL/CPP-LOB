#include <catch2/catch_test_macros.hpp>
#include "../headers/lob.hpp"

TEST_CASE("BID ASK SPREAD TESTCASE #1", "[bid_ask_spread]") {
    // Check Bid Ask Spread for empty order book.
    LOB limitOrderBook = LOB();
    REQUIRE(limitOrderBook.getBidAskSpread() == 0.0);
}

TEST_CASE("BID ASK SPREAD TESTCASE #2", "[bid_ask_spread]") {
    // Check Bid Ask Spread for bids and no asks.
    LOB limitOrderBook = LOB();
    Price testPrice = 34.9;
    limitOrderBook.submitOrder(Order {
        1,
        testPrice,
        1,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC

    });
    REQUIRE(limitOrderBook.getBidAskSpread() == (testPrice * -1.0));
}

TEST_CASE("BID ASK SPREAD TESTCASE #3", "[bid_ask_spread]") {
    // Check Bid Ask Spread for asks and no bids.
    LOB limitOrderBook = LOB();
    Price testPrice = 34.9;
    limitOrderBook.submitOrder(Order {
        1,
        testPrice,
        1,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC

    });
    REQUIRE(limitOrderBook.getBidAskSpread() == testPrice);
}

TEST_CASE("BID ASK SPREAD TESTCASE #4", "[bid_ask_spread]") {
    // Check Bid Ask Spread for asks and no bids.
    LOB limitOrderBook = LOB();
    Price testPriceBid = 34.9;
    Price testPriceAsk = 54.7;
    limitOrderBook.submitOrder(Order {
        1,
        testPriceAsk,
        1,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC

    });
    limitOrderBook.submitOrder(Order {
        2,
        testPriceBid,
        1,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC

    });
    REQUIRE(limitOrderBook.getBidAskSpread() == (testPriceAsk - testPriceBid));
}