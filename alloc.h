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
    static void  deallocate(void* p) {
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
        return nullptr;
    }
    static void* reallocHandle(std::size_t) {
        return nullptr;
    }
};


typedef MallocAlloc<0> alloc;

template <class T, class Alloc>
class SimpleAlloc {
public:
    static T* allocate(std::size_t n) {
        return static_cast<T*>(Alloc::allocate(n * sizeof(T)));
    }

    static void deallocate(T* p) {
        Alloc::deallocate(p);
    }

    static T* reallocate(T* p, std::size_t n ) {
        return Alloc::realocate(p, n);
    }
};

template <int inst>
class FreeListAlloc {
private:

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
