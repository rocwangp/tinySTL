#pragma once

#include "algorithm.h"

namespace tinystl
{

    template <class InputIt, class T, class BinaryOperation>
    T accumulate(InputIt first, InputIt last, T init, BinaryOperation op)
    {
        tinystl::for_each(first, last, [&init, &op](const auto& value) {
                       init = op(init, value); 
                    });
        return init;
    }
    template<class InputIt, class T>
    T accumulate(InputIt first, InputIt last, T init)
    {
        return tinystl::accumulate(first, last, init, [&init](const auto& lhs, const auto& rhs) {
                        init = lhs + rhs;
                        return init;
                    });
    }
}
