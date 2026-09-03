# 📊 CPP-LOB

<div align="center">
  <img src="images/lob_background.jpg" alt="Background" width="75%"/>
  <br><br>
</div>

# 👀 Overview
This is a highly optimised C++ repository of a [Limit Order Book Matching Engine](https://medium.com/@samiur1998/what-is-a-limit-order-book-f48f32915036).

This codebase has been developed using C++17 and uses [Catch2](https://github.com/catchorg/Catch2) as a testing framework.

This project has been throughly tested with 36 test cases and over 500 assertions.
```
All tests passed (571 assertions in 36 test cases)
```

The following optimisations have been incorporated in this project:
- **Red-Black Tree:** Used to store bids and asks and efficiently enforce ranking.
- **Reserving Capacity for Vectors:** Reduces memory overhead for per order dynamic heap allocation.
- **Intrusive Linked List**: Reduces memory allocations, elimanates cache thrashing, and provides true O(1) deletion. 
- **Ordered Map:** Used for effecient O(1) order lookup and delete.
- **Custom Pool Allocator:** Improves speed of memory allocation and reduces memory fragmentation.
- **Memory Optimised Data Ordering:** Data within structs are organized from largest to smallest size, eliminating or reducing alignment padding waste.

With the optimisations above the following benchmarks have been achieved on a standard MacBook Air with an [Apple M4](https://en.wikipedia.org/wiki/Apple_M4) memory chip and 16GB of memory, compiled with **AppleClang 17.0.0.17000013**:

```
Throughput:  6.71796 M ops/sec
P50 Latency: 125 ns
P75 Latency: 167 ns
P90 Latency: 209 ns
P99 Latency: 375 ns
p99.9 Latency: 584 ns
```

# 📄 API
```
submitOrder -> Add a new Order to Order Book.
cancelOrder -> Cancel pre-existing Order in Order Book.
getBestBid -> Get Best Bid in the Order Book.
getBestAsk -> Get Best Ask in the Order Book.
getBidOrderDepths -> Get Bids in the Order Book by Price Level.
getAskOrderDepths -> Get Asks in the Order Book by Price Level.
getBidAskSpread -> Calculates and returns the Bid Ask Spread of the Order Book.
getMidPrice -> Calculates and returns the Mid Price of the Order Book.
getOrderImbalance -> Calculates and returns the Order Imbalance of the Order Book.
```

# 🛠️ Build
The project requires Cmake and C++17 to build successfully.

In order to build run the following commands in sequence:
```
cmake -B build
cmake --build build
```

To run Unit Tests:

```
./build/unit_tests
```

To run BenchMarks:

```
./build/bench
```

# 🤝 Usage & Contribution
Suggestions, Usage, and Contributions are welcomed in this project, with adherence to the [LICENSE](./LICENSE)