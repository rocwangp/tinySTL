#pragma once

#include <cstdint>

namespace tinystl
{

template <class Key>
struct hash
{
    std::size_t operator()(const Key& key)
    {
        return std::size_t(-1);
    }
};


}
