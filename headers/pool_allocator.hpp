#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include "types.hpp"

/* 
    A minimal fixed-block pool allocator, intended for node-based containers
    (std::list, std::map) where every allocation is for exactly one node of a
    fixed size. Freed nodes go back onto a free-list and are handed out again
    on the next allocation, instead of round-tripping through malloc/free.

    State is shared per-T via a function-local static, so every std::list (one
    per price level) allocating LevelOrder nodes draws from the same pool -
    a node freed when one price level empties can be reused by another.

    Not thread-safe. Fine for a single-threaded matching engine; would need a
    lock or thread-local pools if the engine becomes multi-threaded.
*/

template <typename T>
class PoolAllocator {
public:
    using value_type = T;

    PoolAllocator() noexcept = default;

    template <typename U>
    PoolAllocator(const PoolAllocator<U>&) noexcept {}

    T* allocate(SizeT n) {
        if (n != 1) {
            return static_cast<T*>(::operator new(n * sizeof(T)));
        }
        auto& fl = freeList();
        if (fl.empty()) {
            growPool();
        }
        T* p = fl.back();
        fl.pop_back();
        return p;
    }

    void deallocate(T* p, SizeT n) noexcept {
        if (n != 1) {
            ::operator delete(p);
            return;
        }
        freeList().push_back(p);
    }

    template <typename U>
    bool operator==(const PoolAllocator<U>&) const noexcept { return true; }
    template <typename U>
    bool operator!=(const PoolAllocator<U>&) const noexcept { return false; }

private:
    static constexpr SizeT kBlockGrowth = 4096;

    static std::vector<T*>& freeList() {
        static std::vector<T*> fl;
        return fl;
    }

    // Raw, uninitialized storage blocks. We never default-construct T here -
    // the container's allocator_traits::construct placement-news into the
    // slots we hand out. We own the raw bytes so they can be freed on the
    // rare occasion it matters, but for a process-lifetime engine pool we
    // simply let them live until exit, same as most arena allocators.
    static std::vector<T*>& rawBlocks() {
        static std::vector<T*> blocks;
        return blocks;
    }

    static void growPool() {
        T* block = static_cast<T*>(::operator new(kBlockGrowth * sizeof(T)));
        rawBlocks().push_back(block);
        auto& fl = freeList();
        fl.reserve(fl.size() + kBlockGrowth);
        for (SizeT i = 0; i < kBlockGrowth; ++i) {
            fl.push_back(block + i);
        }
    }
};