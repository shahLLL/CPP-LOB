#include <catch2/catch_test_macros.hpp>
#include "../headers/lob.hpp"

TEST_CASE("BID ORDER DEPTHS TESTCASE #1", "[get_bid_order_depths]") {
    // Check bid order depth as bids are added lineraly (lowest price -> highest price)
    Price testPrice1 = 5220;
    Price testPrice2 = 5330;
    Price testPrice3 = 5540;
    Price testPrice4 = 5680;
    Price testPrice5 = 7890;
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
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        testPrice2,
        testQuantity2,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        testPrice3,
        testQuantity3,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        testPrice4,
        testQuantity4,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        testPrice5,
        testQuantity5,
        std::chrono::system_clock::now(),
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
    Price testPrice1 = 5220;
    Price testPrice2 = 5330;
    Price testPrice3 = 5540;
    Price testPrice4 = 5680;
    Price testPrice5 = 7890;
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
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        testPrice2,
        testQuantity2,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        testPrice3,
        testQuantity3,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        testPrice4,
        testQuantity4,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        testPrice5,
        testQuantity5,
        std::chrono::system_clock::now(),
        Side::SELL,
        OrderType::LIMIT,
        TimeInForce::GTC
    });

    REQUIRE(limitOrderBook.getBidOrderDepths(numberOfBids).empty());
}

TEST_CASE("BID ORDER DEPTHS TESTCASE #3", "[get_bid_order_depths]") {
    // Check bid order depth as bids are added non-lineraly (no preset pattern)
    Price testPrice1 = 12120;
    Price testPrice2 = 5330;
    Price testPrice3 = 15240;
    Price testPrice4 = 5680;
    Price testPrice5 = 7890;
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
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        2,
        testPrice2,
        testQuantity2,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        3,
        testPrice3,
        testQuantity3,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        4,
        testPrice4,
        testQuantity4,
        std::chrono::system_clock::now(),
        Side::BUY,
        OrderType::LIMIT,
        TimeInForce::GTC
    });
    limitOrderBook.submitOrder(Order {
        5,
        testPrice5,
        testQuantity5,
        std::chrono::system_clock::now(),
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