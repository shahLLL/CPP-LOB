#include <catch2/catch_test_macros.hpp>
#include "../headers/lob.hpp"

TEST_CASE("ASK ORDER DEPTHS TESTCASE #1", "[get_ask_order_depths]") {
    // Check ask order depth as asks are added lineraly (lowest price -> highest price)
    Price testPrice1 = 1220;
    Price testPrice2 = 1330;
    Price testPrice3 = 1540;
    Price testPrice4 = 1680;
    Price testPrice5 = 2890;
    Quantity testQuantity1 = 4;
    Quantity testQuantity2 = 2;
    Quantity testQuantity3 = 3;
    Quantity testQuantity4 = 14;
    Quantity testQuantity5 = 9;
    SizeT numberOfAsks = 5;

    LOB limitOrderBook = LOB();
    REQUIRE(limitOrderBook.getAskOrderDepths(numberOfAsks).empty());
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

    REQUIRE(!limitOrderBook.getAskOrderDepths(numberOfAsks).empty());
    Levels askOrderDepth = limitOrderBook.getAskOrderDepths(numberOfAsks);
    REQUIRE(askOrderDepth.at(0).price == testPrice1);
    REQUIRE(askOrderDepth.at(0).quantity == testQuantity1);
    REQUIRE(askOrderDepth.at(1).price == testPrice2);
    REQUIRE(askOrderDepth.at(1).quantity == testQuantity2);
    REQUIRE(askOrderDepth.at(2).price == testPrice3);
    REQUIRE(askOrderDepth.at(2).quantity == testQuantity3);
    REQUIRE(askOrderDepth.at(3).price == testPrice4);
    REQUIRE(askOrderDepth.at(3).quantity == testQuantity4);
    REQUIRE(askOrderDepth.at(4).price == testPrice5);
    REQUIRE(askOrderDepth.at(4).quantity == testQuantity5);
}

TEST_CASE("ASK ORDER DEPTHS TESTCASE #2", "[get_ask_order_depths]") {
    // Check for empty ask order depth when Buy side orders added
    Price testPrice1 = 1220;
    Price testPrice2 = 1330;
    Price testPrice3 = 1540;
    Price testPrice4 = 1680;
    Price testPrice5 = 2890;
    Quantity testQuantity1 = 4;
    Quantity testQuantity2 = 2;
    Quantity testQuantity3 = 3;
    Quantity testQuantity4 = 14;
    Quantity testQuantity5 = 9;
    SizeT numberOfAsks = 5;

    LOB limitOrderBook = LOB();
    REQUIRE(limitOrderBook.getAskOrderDepths(numberOfAsks).empty());
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

    REQUIRE(limitOrderBook.getAskOrderDepths(numberOfAsks).empty());
}

TEST_CASE("ASK ORDER DEPTHS TESTCASE #3", "[get_ask_order_depths]") {
    // Check ask order depth as asks are added non-lineraly (no preset pattern)
    Price testPrice1 = 1220;
    Price testPrice2 = 1330;
    Price testPrice3 = 140;
    Price testPrice4 = 180;
    Price testPrice5 = 2890;
    Quantity testQuantity1 = 4;
    Quantity testQuantity2 = 2;
    Quantity testQuantity3 = 3;
    Quantity testQuantity4 = 14;
    Quantity testQuantity5 = 9;
    SizeT numberOfAsks = 5;

    LOB limitOrderBook = LOB();
    REQUIRE(limitOrderBook.getAskOrderDepths(numberOfAsks).empty());
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

    REQUIRE(!limitOrderBook.getAskOrderDepths(numberOfAsks).empty());
    Levels askOrderDepth = limitOrderBook.getAskOrderDepths(numberOfAsks);
    REQUIRE(askOrderDepth.at(0).price == testPrice3);
    REQUIRE(askOrderDepth.at(0).quantity == testQuantity3);
    REQUIRE(askOrderDepth.at(1).price == testPrice4);
    REQUIRE(askOrderDepth.at(1).quantity == testQuantity4);
    REQUIRE(askOrderDepth.at(2).price == testPrice1);
    REQUIRE(askOrderDepth.at(2).quantity == testQuantity1);
    REQUIRE(askOrderDepth.at(3).price == testPrice2);
    REQUIRE(askOrderDepth.at(3).quantity == testQuantity2);
    REQUIRE(askOrderDepth.at(4).price == testPrice5);
    REQUIRE(askOrderDepth.at(4).quantity == testQuantity5);
}