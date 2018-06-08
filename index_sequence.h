#pragma once
#include <iostream>

namespace tinystl
{
    template <std::size_t... Idx>
    struct index_sequence
    {
    };

    template <std::size_t N, std::size_t... Idx>
    struct make_index_sequence : public make_index_sequence<N-1, N-1, Idx...>
    {
    };

    template <std::size_t... Idx>
    struct make_index_sequence<0, Idx...> : public index_sequence<Idx...>
    {
    };
}
