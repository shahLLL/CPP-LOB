#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include <iostream>
#include <string>
#include "../headers/pool_allocator.hpp"

const std::string destructorMessage = "Destructor called";

// Custom struct to test pool allocator
struct PoolTest {
    std::uint64_t data1;
    std::int64_t data2;
    std::uint8_t data3;
    double data4;

    PoolTest(std::uint64_t data1, std::int64_t data2,  std::uint8_t data3, 
        double data4): data1(data1), data2(data2), data3(data3), data4(data4) 
        {};

    ~PoolTest() {
        std::cout << destructorMessage << " " <<
            data1 << " " <<
            data2 << " " <<
            (int)data3 << " " <<
            data4 << std::endl;
    }
};

TEST_CASE("POOL ALLOCATOR TESTCASE #1", "[pool_allocator]") {
    PoolAllocator<PoolTest> allocator = PoolAllocator<PoolTest>();
    PoolTest *testPtr1 = allocator.allocate();
    PoolTest *testPtr2 = allocator.allocate();
    PoolTest *testPtr3 = allocator.allocate();
    
    testPtr1->data1 = 23;
    testPtr1->data2 = 45;
    testPtr1->data3 = 3;
    testPtr1->data4 = 67.9;

    REQUIRE(testPtr1->data1 == 23);
    REQUIRE(testPtr1->data2 == 45);
    REQUIRE(testPtr1->data3 == 3);
    REQUIRE(testPtr1->data4 == 67.9);

    testPtr2->data1 = 12;
    testPtr2->data2 = -39;
    testPtr2->data3 = 1;
    testPtr2->data4 = 243.1;

    REQUIRE(testPtr2->data1 == 12);
    REQUIRE(testPtr2->data2 == -39);
    REQUIRE(testPtr2->data3 == 1);
    REQUIRE(testPtr2->data4 == 243.1);

    testPtr3->data1 = 1998;
    testPtr3->data2 = -1995;
    testPtr3->data3 = 200;
    testPtr3->data4 = 2000.02;

    REQUIRE(testPtr3->data1 == 1998);
    REQUIRE(testPtr3->data2 == -1995);
    REQUIRE(testPtr3->data3 == 200);
    REQUIRE(testPtr3->data4 == 2000.02);

    allocator.deallocate(testPtr1);
    allocator.deallocate(testPtr2);
    allocator.deallocate(testPtr3);

    // Validate destructor not called for deallocate.
    REQUIRE(testPtr1->data1 == 23);
    REQUIRE(testPtr1->data2 == 45);
    REQUIRE(testPtr1->data3 == 3);
    REQUIRE(testPtr1->data4 == 67.9);

    REQUIRE(testPtr2->data1 == 12);
    REQUIRE(testPtr2->data2 == -39);
    REQUIRE(testPtr2->data3 == 1);
    REQUIRE(testPtr2->data4 == 243.1);

    REQUIRE(testPtr3->data1 == 1998);
    REQUIRE(testPtr3->data2 == -1995);
    REQUIRE(testPtr3->data3 == 200);
    REQUIRE(testPtr3->data4 == 2000.02);
}

TEST_CASE("POOL ALLOCATOR TESTCASE #2", "[pool_allocator]") {
    Catch::RedirectedStdOut redirectedOutput;
    PoolAllocator<PoolTest> allocator = PoolAllocator<PoolTest>();

    PoolTest *testPtr1 = allocator.create(1, 22, 33, 444.45);
    REQUIRE(testPtr1->data1 == 1);
    REQUIRE(testPtr1->data2 == 22);
    REQUIRE(testPtr1->data3 == 33);
    REQUIRE(testPtr1->data4 == 444.45);

    PoolTest *testPtr2 = allocator.create(9, -61, 0, -90.87);
    REQUIRE(testPtr2->data1 == 9);
    REQUIRE(testPtr2->data2 == -61);
    REQUIRE(testPtr2->data3 == 0);
    REQUIRE(testPtr2->data4 == -90.87);

    allocator.destroy(testPtr1);
    std::string destructorOutput1 = redirectedOutput.str();
    REQUIRE(destructorOutput1 == "Destructor called 1 22 33 444.45\n");

    allocator.destroy(testPtr2);
    std::string destructorOutput2 = redirectedOutput.str();
    REQUIRE(destructorOutput2 == 
        "Destructor called 1 22 33 444.45\nDestructor called 9 -61 0 -90.87\n");
}