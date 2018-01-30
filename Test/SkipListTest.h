#pragma once

#include "TestUtil.h"

#include "../skiplist.h"
#include "../vector.h"
#include "../string.h"

#include <set>
#include <random>
#include <cassert>

namespace tinystl
{
    namespace SkipListTest
    {
        template <class T>
        using stdS = std::set < T >;
        
        template <class T>
        using tsSL = tinystl::SkipList<T, T, std::_Identity<T>>;

        void testCase1();
        void testCase2();
        void testCase3();
        void testCase4();
        void testCase5();
        void testCase6();
        void testCaseAll();
    }
}
