#pragma once

#include "TestUtil.h"
#include "../map.h"
#include "../string.h"
#include "../vector.h"

#include <map>
#include <random>
#include <cassert>

namespace tinystl
{
    namespace MapTest
    {
        template <class Key, class Value>
        using stdM = std::map<Key, Value>;

        template <class Key, class Value>
        using tsM = tinystl::map<Key, Value>;

        void testCase1();
        void testCase2();
        void testCase3();
        void testCase4();
        void testCase5();
        void testCase6();

        void testCaseAll();
    }
}
