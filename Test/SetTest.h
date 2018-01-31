#pragma once

#include "TestUtil.h"
#include "../set.h"
#include "../string.h"

#include <cassert>
#include <set>
#include <random>

namespace tinystl
{
    namespace SetTest
    {
        template <class T>
        using stdS = std::set < T >;

        template <class T>
        using tsS = tinystl::set < T >;

        void testCase1();
        void testCase2();
        void testCase3();
        void testCase4();
        void testCase5();
        void testCase6();
        void testCase7();

        void testCaseAll();
    }
}
