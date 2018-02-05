#pragma once

#include <cstdint>
#include <malloc.h>
#include "iterator.h"
#include "construct.h"

namespace tinystl {

template <int inst>
class MallocAlloc {
public:
    static void* allocate(std::size_t n){
        void* p = ::malloc(n);
        if(p == nullptr)
            p = mallocHandle(n);
        return p;
    }
    static void  deallocate(void* p, std::size_t /* n */) {
        ::free(p);
    }
    static void* reallocate(void* p, std::size_t n) {
        void* ptr = ::realloc(p, n);
        if(ptr == nullptr)
            ptr = reallocHandle(n);
        return ptr;
    }

private:
    static void* mallocHandle(std::size_t) {
        throw std::runtime_error("none memory");
    }
    static void* reallocHandle(std::size_t) {
        throw std::runtime_error("none memory");
    }
};


template <int inst>
class FreeListAlloc {

public:
    enum  MemoryUnit { MAX_BYTES = 128, ALIGN = 8, N_FreeLists = 16 };
private:
    union FreeListNode {
        FreeListNode* next;
    };
private:
    static FreeListNode *freeList_[MemoryUnit::N_FreeLists];
    static char* start_;
    static char* finish_;
public:
    static void* allocate(std::size_t n) {
        if(n > MemoryUnit::MAX_BYTES)
            return MallocAlloc<0>::allocate(n);
        FreeListNode* result = freeList_[FREELIST_INDEX(n)];
        if(!result)
            return refill(ROUND_UP(n));
        freeList_[FREELIST_INDEX(n)] = result->next; 
        return result;
    }
    static void deallocate(void* p, std::size_t n) {
        if(n > MemoryUnit::MAX_BYTES)
            return MallocAlloc<0>::deallocate(p, n);
        FreeListNode* result = freeList_[FREELIST_INDEX(n)];
        if(p)
        {
            static_cast<FreeListNode*>(p)->next = result;
            freeList_[FREELIST_INDEX(n)] = static_cast<FreeListNode*>(p);
        }
    }
private:
    static std::size_t ROUND_UP(std::size_t n)
    {
        return (n + (MemoryUnit::ALIGN) - 1) & (~(MemoryUnit::ALIGN - 1));
    }
    static std::size_t FREELIST_INDEX(std::size_t n)
    {
        return (n + MemoryUnit::ALIGN - 1) / MemoryUnit::ALIGN - 1;
    }
    static void* refill(std::size_t n)
    {
        int nfreelists = 20;
        char* chunk = chunk_alloc(n, nfreelists);
        if(nfreelists == 1)
            return chunk;
        else
        {
            freeList_[FREELIST_INDEX(n)] = (FreeListNode*)(chunk + n);
            FreeListNode* cur = freeList_[FREELIST_INDEX(n)];
            for(int i = 0; i < nfreelists - 1; ++i)
            {
                if(i != nfreelists - 2)
                    cur->next = (FreeListNode*)((char*)(cur) + n);
                else
                    cur->next = nullptr;
                cur = cur->next;
            }
            return chunk;
        }
    }
    static char* chunk_alloc(std::size_t n, int& nfreelists)
    {
        std::size_t totalBytes = n * nfreelists;
        std::size_t leftBytes = finish_ - start_;
        if(leftBytes >= totalBytes)
        {
            char* result = start_;
            start_ += totalBytes;
            return result;
        }
        else if(leftBytes >= n)
        {
            nfreelists = leftBytes / n;
            char* result = start_;
            start_ += n * nfreelists;
            return result;
        }
        else
        {
            if(leftBytes > 0)
            {
                ((FreeListNode*)(start_))->next = freeList_[FREELIST_INDEX(leftBytes)];
                freeList_[FREELIST_INDEX(leftBytes)] = (FreeListNode*)(start_);
            }
            start_ = static_cast<char*>(::malloc(2 * totalBytes));
            if(start_ == nullptr)
            {
                for(std::size_t i = n; i <= MemoryUnit::MAX_BYTES ; i += MemoryUnit::ALIGN)
                {
                    void *p = freeList_[FREELIST_INDEX(i)];
                    if(p != nullptr)
                    {
                        start_ = static_cast<char*>(p);
                        finish_ = start_ + i;
                        freeList_[FREELIST_INDEX(i)] = static_cast<FreeListNode*>(p)->next;
                        return chunk_alloc(n, nfreelists);
                    }
                }
                finish_ = 0;
                start_ = static_cast<char*>(MallocAlloc<0>::allocate(2 * totalBytes));
            }
            finish_ = start_ + 2 * totalBytes;
            return chunk_alloc(n, nfreelists);
        }
    }
};

template <int inst>
typename FreeListAlloc<inst>::FreeListNode* 
FreeListAlloc<inst>::freeList_[FreeListAlloc<inst>::MemoryUnit::N_FreeLists] = 
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <int inst>
char* FreeListAlloc<inst>::start_ = 0;
template <int inst>
char* FreeListAlloc<inst>::finish_ = 0;

/* typedef MallocAlloc<0> alloc; */
typedef FreeListAlloc<0> alloc;

template <class T, class Alloc>
class SimpleAlloc {
public:
    static T* allocate(std::size_t n) {
        return static_cast<T*>(Alloc::allocate(n * sizeof(T)));
    }

    static void deallocate(T* p, std::size_t n = sizeof(T)) {
        Alloc::deallocate(p, n);
    }

    static T* reallocate(T* p, std::size_t n ) {
        return Alloc::realocate(p, n);
    }
};



template <class ForwardIterator, class Size, class T>
T* uninitializedFill(ForwardIterator it, Size n, const T& value) {
    while(n--)
        construct(&*it++, value);
    return it;
}

template <class InputIterator, class OutputIterator>
OutputIterator uninitializedCopy(InputIterator front, InputIterator back, OutputIterator target)
{
    typedef typename iterator_traits<OutputIterator>::value_type valueType;
    const auto n = back - front;
    for(int i = 0; i < n; ++i)
        construct(&*(target + i), valueType());
    std::copy(front, back, target);
    return target;
}

}
