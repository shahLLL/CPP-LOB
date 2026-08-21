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

Levels Engine::getBidOrderDepths(SizeT n) const {
    Levels bidDepths;
    bidDepths.reserve(n);
    for(auto& [price, priceLevel] : bids) {
        if(bidDepths.size() >= n) break;
        bidDepths.push_back(Level{ price, priceLevel.totalQuantity });
    }
    return bidDepths;
}

Levels Engine::getAskOrderDepths(SizeT n) const {
    Levels askDepths;
    askDepths.reserve(n);
    for(auto& [price, priceLevel] : asks) {
        if(askDepths.size() >= n) break;
        askDepths.push_back(Level{ price, priceLevel.totalQuantity });
    }
    return askDepths;
}

Price Engine::getBidAskSpread() const {
    if(bids.empty() && asks.empty()) return 0.0;
    if(bids.empty() && !asks.empty()) return asks.begin()->first;
    if(!bids.empty() && asks.empty()) return (bids.begin()->first) * -1.0;
    return (asks.begin()->first) - (bids.begin()->first);
}

Price Engine::getMidPrice() const {
    if(bids.empty() && asks.empty()) return 0.0;
    if(bids.empty() && !asks.empty()) return asks.begin()->first;
    if(!bids.empty() && asks.empty()) return bids.begin()->first;
    return ((bids.begin()->first) + (asks.begin()->first) / 2.0);
}

Double Engine::getOrderImbalance() const {
    Double bidTotal = 0.0;
    Double askTotal = 0.0;
    for(auto& [price, priceLevel] : bids) { 
        bidTotal += static_cast<Double>(priceLevel.totalQuantity);
    }
    for(auto& [price, priceLevel] : asks) { 
        askTotal += static_cast<Double>(priceLevel.totalQuantity);
    }
    if(bidTotal + askTotal == 0.0) return 0.0;
    return (bidTotal - askTotal) / (bidTotal + askTotal);
}