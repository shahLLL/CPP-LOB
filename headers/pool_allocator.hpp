#pragma once

#include <cstddef>
#include <utility>
#include <vector>

/* A fixed-block object pool for T. Hands out T* from pre-allocated arenas
    and recycles freed ones via a free-list, instead of round-tripping every
    allocation through malloc/free.

    Owned as an instance (e.g. a member of LOB), not shared via global static
    state - one pool per owner, explicit lifetime. Non-copyable and
    non-movable

    Not thread-safe. Fine for a single-threaded matching engine, would need a
    lock or thread-local pools if the engine becomes multi-threaded. */

template <typename T>
class PoolAllocator {
public:
    PoolAllocator() = default;

    PoolAllocator(const PoolAllocator&) = delete;
    PoolAllocator& operator=(const PoolAllocator&) = delete;
    PoolAllocator(PoolAllocator&&) = delete;
    PoolAllocator& operator=(PoolAllocator&&) = delete;

    ~PoolAllocator() {
        for (T* block : rawBlocks_) {
            ::operator delete(block);
        }
    }

    T* allocate() {
        if (freeList_.empty()) {
            growPool();
        }
        T* p = freeList_.back();
        freeList_.pop_back();
        return p;
    }

    void deallocate(T* p) noexcept {
        freeList_.push_back(p);
    }

    template <typename... Args>
    T* create(Args&&... args) {
        T* p = allocate();
        new (p) T(std::forward<Args>(args)...);
        return p;
    }

    void destroy(T* p) noexcept {
        p->~T();
        deallocate(p);
    }

private:
    static constexpr std::size_t kBlockGrowth = 4096;

    std::vector<T*> freeList_;
    std::vector<T*> rawBlocks_;

    void growPool() {
        T* block = static_cast<T*>(::operator new(kBlockGrowth * sizeof(T)));
        rawBlocks_.push_back(block);
        freeList_.reserve(freeList_.size() + kBlockGrowth);
        for (std::size_t i = 0; i < kBlockGrowth; ++i) {
            freeList_.push_back(block + i);
        }
    }
};