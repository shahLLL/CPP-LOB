#include "../headers/engine.hpp"

TOB Engine::getBestBid() const noexcept {
    auto bidsItr = bids.begin();
    if(bids.empty()) return std::nullopt;
    return Level{ bidsItr->first, bidsItr->second.totalQuantity };
}

TOB Engine::getBestAsk() const noexcept {
    auto asksItr = asks.begin();
    if(asks.empty()) return std::nullopt;
    return Level{ asksItr->first, asksItr->second.totalQuantity };
}