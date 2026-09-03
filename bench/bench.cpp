#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include "../headers/lob.hpp"

// Constants
constexpr SizeT oneMillion = 1'000'000;
constexpr SizeT oneHundredThousand = 100'000;
constexpr unsigned long long seed = 0xCAFEBABE;
constexpr double tenExpNegativeNine = 1e-9;
constexpr double tenExpThree = 1e3;
constexpr double tenExpSix = 1e6;
constexpr double pointFive = 0.5;
constexpr double pointSevenFive = 0.75;
constexpr double pointNine = 0.9;
constexpr double pointNineNine = 0.99;
constexpr double pointNineNineNine = 0.999;
constexpr double price90 = 90.0;
constexpr double price110 = 110.0;
TimeStamp dummyTimeStamp =  std::chrono::system_clock::now();

// Type Aliases
using UniformSmallIntDistribution = std::uniform_int_distribution<int>;
using UniformSignedIntDistribution = std::uniform_int_distribution<std::int64_t>;
using UniformUnsignedIntDistribution = std::uniform_int_distribution<std::uint64_t>;
using UniformDoubleDistribution = std::uniform_real_distribution<double>;
using UniformSizeTDistribution = std::uniform_int_distribution<SizeT>;
using IDs = std::vector<std::uint64_t>;
using Latencies = std::vector<long long>;
using Nanoseconds = std::chrono::nanoseconds;

/*
    Algortihmic order-flow generator where distribution is modelled on
    real exchange data. Composed of:
    1. Passive limit orders (resting, spread across a tight price band) -> 60%
    2. Aggressive limit orders (cross the spread → immediate fills) -> 20%
    3. Market orders -> 10%
    4. Cancel requests (randomly pick a live order ID) -> 10%
*/

int main() {
    const SizeT n = oneMillion;

    LOB limitOrderBook = LOB();
    std::mt19937_64 rng(seed);

    UniformSmallIntDistribution sideDistribution(0, 1);
    UniformDoubleDistribution priceDistribution(95.0, 105.0);
    UniformSignedIntDistribution quantityDistribution(1, 100);
    UniformSmallIntDistribution typeDistribution(0, 9);

    IDs liveIDs;
    liveIDs.reserve(n / 2);
    ID currentID = 1;

    Latencies latencies;
    latencies.reserve(n);

    // Populate Limit Order Book with passive orders.
    for(int i = 0; i < 200; i++) {
        Events passiveOrderEvents = limitOrderBook.submitOrder(Order {
            currentID,
            priceDistribution(rng),
            quantityDistribution(rng),
            dummyTimeStamp,
            sideDistribution(rng) ? Side::BUY : Side::SELL,
            OrderType::LIMIT,
            TimeInForce::GTC
        });

        for (auto& e: passiveOrderEvents)
            if(e.eventType == EventType::REST)
                liveIDs.push_back(currentID);
        currentID = currentID + 1;
    }

    // Timed loop
    auto wallStart = std::chrono::high_resolution_clock::now();

    for(SizeT i = 0; i < n; i++) {
        auto timeZero = std::chrono::high_resolution_clock::now();
        auto typeIndicator = typeDistribution(rng);

        if(typeIndicator < 6) {
            // Passive Limit Order
            Events passiveOrderEvents = limitOrderBook.submitOrder(Order {
                currentID,
                priceDistribution(rng),
                quantityDistribution(rng),
                dummyTimeStamp,
                sideDistribution(rng) ? Side::BUY : Side::SELL,
                OrderType::LIMIT,
                TimeInForce::GTC
            });

            for(auto& e : passiveOrderEvents)
                if(e.eventType == EventType::REST)
                    liveIDs.push_back(currentID);
        } else if (typeIndicator < 8) {
            // Aggresive Limit Order
            Side orderSide = sideDistribution(rng) ? Side::BUY : Side::SELL;
            limitOrderBook.submitOrder(Order {
                currentID,
                orderSide == Side::BUY ? price110 : price90,
                quantityDistribution(rng),
                dummyTimeStamp,
                orderSide,
                OrderType::LIMIT,
                TimeInForce::IOC
            });
        } else if (typeIndicator < 9) {
            // Market Order
            limitOrderBook.submitOrder(Order {
                currentID,
                0.0,
                quantityDistribution(rng),
                dummyTimeStamp,
                sideDistribution(rng) ? Side::BUY : Side::SELL,
                OrderType::MARKET,
                TimeInForce::NON
            });
        } else {
            // Cancel Order
            if(liveIDs.empty()) continue;
            UniformSizeTDistribution cancelDistribution(0, liveIDs.size() - 1);
            SizeT cancelIndex = cancelDistribution(rng);
            ID cancelID = liveIDs[cancelIndex];
            std::swap(liveIDs[cancelIndex], liveIDs.back());
            liveIDs.pop_back();
            limitOrderBook.cancelOrder(cancelID, dummyTimeStamp);
        }

        auto timeOne = std::chrono::high_resolution_clock::now();
        latencies.push_back(std::chrono::duration_cast<Nanoseconds>(timeOne - 
            timeZero).count());
        currentID = currentID + 1;
    }

    auto wallEnd = std::chrono::high_resolution_clock::now();

    // Calculate Results
    Double elapsedTime = std::chrono::duration_cast<Nanoseconds>(wallEnd - 
        wallStart).count() * tenExpNegativeNine;
    Double throughput = n / elapsedTime;
    
    std::sort(latencies.begin(), latencies.end());
    auto p50  = latencies[n * pointFive];
    auto p75  = latencies[n * pointSevenFive];
    auto p90  = latencies[n * pointNine];
    auto p99  = latencies[n * pointNineNine];
    auto p999 = latencies[n * pointNineNineNine];

    // Output Results
    std::cout << "============================================" << std::endl;
    std::cout << "  Matching Engine Benchmark  (" << n << " ops)" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "  Elapsed:     " << elapsedTime * tenExpThree << " ms" << std::endl;
    std::cout << "  Throughput:  " << throughput / tenExpSix   << " M ops/sec" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "  Latency (per op, including chrono overhead ~10 ns):" << std::endl;
    std::cout << "    p50  : " << p50  << " ns" << std::endl;
    std::cout << "    p75  : " << p75  << " ns" << std::endl;
    std::cout << "    p90  : " << p90  << " ns" << std::endl;
    std::cout << "    p99  : " << p99  << " ns" << std::endl;
    std::cout << "    p99.9: " << p999 << " ns" << std::endl;
    std::cout << "============================================" << std::endl;

    return 0;
}