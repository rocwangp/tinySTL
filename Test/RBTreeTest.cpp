#include <iostream>

#include <cassert>
#include <random>
#include <utility>
#include "RBTreeTest.h"
#include "../vector.h"
#include "TestUtil.h"

#include <set>
namespace tinystl
{
    namespace RBTreeTest 
    {
        void testCase1()
        {
            tsRBT<int, int> rb;
            std::multiset<int> v;
            std::random_device rd; 
            for(int i = 0; i < 10; ++i)
            {
                auto r = rd() % 65536;
                v.insert(r);
                auto it = rb.insert_multi(r);
                assert((*it) == r);
            }
            assert(tinystl::Test::container_equal(rb, v));
        }
        void testCase2()
        {
            tsRBT<int, int> rb;
            std::multiset<int> stdS;
            std::random_device rd;
            std::vector<int> v;
            int n = 0;
            for(int i = 0; i < 100; ++i)
            {
                auto r = rd() % 10000;
                if(std::find(v.begin(), v.end(), r) != v.end())
                    continue;
                v.push_back(r);
                rb.insert_multi(r);
                stdS.insert(r);
                assert(rb.isBalance());
                assert(tinystl::Test::container_equal(rb, stdS));
            }
            /* rb.insert_multi(v[0]); */
            /* rb.insert_multi(v[0]); */
            /* stdS.insert(v[0]); */
            /* stdS.insert(v[0]); */
            /* rb.insert_multi(v[1]); */
            /* stdS.insert(v[1]); */
            assert(tinystl::Test::container_equal(rb, stdS));
            while(!stdS.empty())
            {
                int idx = rd() % (v.size());
                n = v[idx];
                std::cout << "erase " << n << std::endl;
                v.erase(v.begin() + idx);
                rb.erase(n);
                stdS.erase(n);
                tinystl::Test::print(stdS);
                tinystl::Test::print(rb);
                rb.print_inOrder();
                rb.print_levelOrder();
                assert(rb.isBalance());
                assert(tinystl::Test::container_equal(rb, stdS));
            }
        }
        void testAllCase()
        {
            testCase1();
            testCase2();
        }
    }
}
