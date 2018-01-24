#pragma once

#include <new>
#include <iostream>
namespace tinystl {

template <class T1, class T2>
inline void construct(T1* p, const T2& value) {
    ::new(p) T2(value);
}

template <class T>
inline void destroy(T* p) {
    p->~T();
}

template <class T>
inline void destroy(T front, T back) {
    for(; front != back; ++front)
        destroy(&*front);
}
}
