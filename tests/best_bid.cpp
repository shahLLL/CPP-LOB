#include <catch2/catch_test_macros.hpp>
#include "../headers/lob.hpp"

TEST_CASE("BEST BID TEST CASE #1", "[get_best_bid]") {
    /*
    Check for best bid when:
    1. There are no bids.
    2. A new bid added
    3. A bid better than the best bid (in terms of price) is added
    4. A bid less than the best bid (in terms of price) is added
    */
    LOB limitOrderBook = LOB();

    Price testPrice1 = 12.5;
    Quantity testQuantity1 = 5;
    Price testPrice2 = testPrice1 / 2.0;
    Quantity testQuantity2 = testQuantity1 * 2;
    Price testPrice3 = testPrice1 * 3.0;
    Quantity testQuantity3 = testQuantity1 - 2;
    Price testPrice4 = testPrice1 * 2.0;
    Quantity testQuantity4 = testQuantity1 - 2;
    
    REQUIRE(!limitOrderBook.getBestBid().has_value());

    limitOrderBook.submitOrder(Order {
        1,
        testPrice1,
        testQuantity1,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(limitOrderBook.getBestBid().has_value());
    REQUIRE(limitOrderBook.getBestBid().value().price == testPrice1);
    REQUIRE(limitOrderBook.getBestBid().value().quantity == testQuantity1);

    limitOrderBook.submitOrder(Order {
        2,
        testPrice2,
        testQuantity2,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(limitOrderBook.getBestBid().has_value());
    REQUIRE(limitOrderBook.getBestBid().value().price == testPrice1);
    REQUIRE(limitOrderBook.getBestBid().value().quantity == testQuantity1);

    limitOrderBook.submitOrder(Order {
        3,
        testPrice3,
        testQuantity3,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(limitOrderBook.getBestBid().has_value());
    REQUIRE(limitOrderBook.getBestBid().value().price == testPrice1);
    REQUIRE(limitOrderBook.getBestBid().value().quantity == testQuantity1);

    limitOrderBook.submitOrder(Order {
        4,
        testPrice4,
        testQuantity4,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(limitOrderBook.getBestBid().has_value());
    REQUIRE(limitOrderBook.getBestBid().value().price == testPrice4);
    REQUIRE(limitOrderBook.getBestBid().value().quantity == testQuantity4);
}

TEST_CASE("BEST BID TEST CASE #2", "[get_best_bid]") {
    // Best Bid not effect by Sell Side orders
    LOB limitOrderBook = LOB();

    Price testPrice1 = 12.5;
    Quantity testQuantity1 = 5;
    Price testPrice2 = testPrice1 / 2.0;
    Quantity testQuantity2 = testQuantity1 * 2;
    Price testPrice3 = testPrice1 * 3.0;
    Quantity testQuantity3 = testQuantity1 - 2;
    Price testPrice4 = testPrice1 * 2.0;
    Quantity testQuantity4 = testQuantity1 - 2;
    
    REQUIRE(!limitOrderBook.getBestBid().has_value());

    limitOrderBook.submitOrder(Order {
        1,
        testPrice1,
        testQuantity1,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(!limitOrderBook.getBestBid().has_value());

    limitOrderBook.submitOrder(Order {
        2,
        testPrice2,
        testQuantity2,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(!limitOrderBook.getBestBid().has_value());

    limitOrderBook.submitOrder(Order {
        3,
        testPrice3,
        testQuantity3,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(!limitOrderBook.getBestBid().has_value());

    limitOrderBook.submitOrder(Order {
        4,
        testPrice4,
        testQuantity4,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    REQUIRE(!limitOrderBook.getBestBid().has_value());
}