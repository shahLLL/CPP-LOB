#include <catch2/catch_test_macros.hpp>
#include "../headers/lob.hpp"

TEST_CASE("BID ORDER DEPTHS TESTCASE #1", "[get_bid_order_depths]") {
    // Check bid order depth as bids are added lineraly (lowest price -> highest price)
    Price testPrice1 = 12.2;
    Price testPrice2 = 13.3;
    Price testPrice3 = 15.4;
    Price testPrice4 = 16.8;
    Price testPrice5 = 28.9;
    Quantity testQuantity1 = 4;
    Quantity testQuantity2 = 2;
    Quantity testQuantity3 = 3;
    Quantity testQuantity4 = 14;
    Quantity testQuantity5 = 9;
    SizeT numberOfBids = 5;

    LOB limitOrderBook = LOB();
    REQUIRE(limitOrderBook.getBidOrderDepths(numberOfBids).empty());
    limitOrderBook.submitOrder(Order {
        1,
        testPrice1,
        testQuantity1,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        testPrice2,
        testQuantity2,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        testPrice3,
        testQuantity3,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        testPrice4,
        testQuantity4,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        testPrice5,
        testQuantity5,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(!limitOrderBook.getBidOrderDepths(numberOfBids).empty());
    Levels bidOrderDepth = limitOrderBook.getBidOrderDepths(numberOfBids);
    REQUIRE(bidOrderDepth.at(0).price == testPrice5);
    REQUIRE(bidOrderDepth.at(0).quantity == testQuantity5);
    REQUIRE(bidOrderDepth.at(1).price == testPrice4);
    REQUIRE(bidOrderDepth.at(1).quantity == testQuantity4);
    REQUIRE(bidOrderDepth.at(2).price == testPrice3);
    REQUIRE(bidOrderDepth.at(2).quantity == testQuantity3);
    REQUIRE(bidOrderDepth.at(3).price == testPrice2);
    REQUIRE(bidOrderDepth.at(3).quantity == testQuantity2);
    REQUIRE(bidOrderDepth.at(4).price == testPrice1);
    REQUIRE(bidOrderDepth.at(4).quantity == testQuantity1);
}

TEST_CASE("BID ORDER DEPTHS TESTCASE #2", "[get_bid_order_depths]") {
    // Check for empty bid order depth when Sell side orders added
    Price testPrice1 = 12.2;
    Price testPrice2 = 13.3;
    Price testPrice3 = 15.4;
    Price testPrice4 = 16.8;
    Price testPrice5 = 28.9;
    Quantity testQuantity1 = 4;
    Quantity testQuantity2 = 2;
    Quantity testQuantity3 = 3;
    Quantity testQuantity4 = 14;
    Quantity testQuantity5 = 9;
    SizeT numberOfBids = 5;

    LOB limitOrderBook = LOB();
    REQUIRE(limitOrderBook.getBidOrderDepths(numberOfBids).empty());
    limitOrderBook.submitOrder(Order {
        1,
        testPrice1,
        testQuantity1,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        testPrice2,
        testQuantity2,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        testPrice3,
        testQuantity3,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        testPrice4,
        testQuantity4,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        testPrice5,
        testQuantity5,
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(limitOrderBook.getBidOrderDepths(numberOfBids).empty());
}

TEST_CASE("BID ORDER DEPTHS TESTCASE #3", "[get_bid_order_depths]") {
    // Check bid order depth as bids are added non-lineraly (no preset pattern)
    Price testPrice1 = 121.2;
    Price testPrice2 = 13.3;
    Price testPrice3 = 152.4;
    Price testPrice4 = 16.8;
    Price testPrice5 = 28.9;
    Quantity testQuantity1 = 4;
    Quantity testQuantity2 = 2;
    Quantity testQuantity3 = 3;
    Quantity testQuantity4 = 14;
    Quantity testQuantity5 = 9;
    SizeT numberOfBids = 5;

    LOB limitOrderBook = LOB();
    REQUIRE(limitOrderBook.getBidOrderDepths(numberOfBids).empty());
    limitOrderBook.submitOrder(Order {
        1,
        testPrice1,
        testQuantity1,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        testPrice2,
        testQuantity2,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        testPrice3,
        testQuantity3,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        testPrice4,
        testQuantity4,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        testPrice5,
        testQuantity5,
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(!limitOrderBook.getBidOrderDepths(numberOfBids).empty());
    Levels bidOrderDepth = limitOrderBook.getBidOrderDepths(numberOfBids);
    REQUIRE(bidOrderDepth.at(0).price == testPrice3);
    REQUIRE(bidOrderDepth.at(0).quantity == testQuantity3);
    REQUIRE(bidOrderDepth.at(1).price == testPrice1);
    REQUIRE(bidOrderDepth.at(1).quantity == testQuantity1);
    REQUIRE(bidOrderDepth.at(2).price == testPrice5);
    REQUIRE(bidOrderDepth.at(2).quantity == testQuantity5);
    REQUIRE(bidOrderDepth.at(3).price == testPrice4);
    REQUIRE(bidOrderDepth.at(3).quantity == testQuantity4);
    REQUIRE(bidOrderDepth.at(4).price == testPrice2);
    REQUIRE(bidOrderDepth.at(4).quantity == testQuantity2);
}