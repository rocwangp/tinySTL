#pragma once

#include <new>
#include <iostream>
namespace tinystl {

template <class T1, class T2>
inline void construct(T1* p, const T2& value) {
    if(p)
        ::new(static_cast<void*>((p))) T1((value));
}

template <class T1, class T2>
inline void construct(T1* p, T2&& value) {
    if(p)
        ::new(p) T1(std::forward<T2>(value));
}


template <class T>
inline void destroy(T* p) {
    if(p)
        p->~T();
}

template <class T>
inline void destroy(T front, T back) {
    for(; front != back; ++front)
        destroy(&*front);
}
}
