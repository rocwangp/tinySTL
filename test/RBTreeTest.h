#pragma once
#include <algorithm>

#include "../rb_tree.h"

namespace tinystl
{
    namespace RBTreeTest
    {
        template <class Type>
        class identity
        {
        public:
            typedef Type   type;
            const Type& operator()(const Type& value) const
            {
                return value;
            }
        };
        template <class Key, class Value>
        using tsRBT = tinystl::RBTree < Key, Value >;

        void testCase1();
        void testCase2();

        void testAllCase();
    }
}
