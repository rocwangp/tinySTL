#include "SkipListTest.h"

namespace tinystl
{
    namespace SkipListTest
    {
        void testCase1()
        {
            stdS<int> s1;
            tsSL<int> s2;
            assert(tinystl::Test::container_equal(s1, s2));
            
            int arr[] = {1, 4, 2, 5, 7, 11, 15, 13, 22, 11, 3, 7};
            stdS<int> s3(std::begin(arr), std::end(arr));
            tsSL<int> s4(std::begin(arr), std::end(arr));
            assert(tinystl::Test::container_equal(s3, s4));
            
            auto s5(s3);
            auto s6(s4);
            assert(tinystl::Test::container_equal(s5, s6));

            stdS<int> s7;
            tsSL<int> s8;
            s7 = s5;
            s8 = s6;
            assert(tinystl::Test::container_equal(s7, s8));

            auto s9(std::move(s7));
            auto s10(std::move(s8));
            assert(tinystl::Test::container_equal(s7, s8));
            assert(tinystl::Test::container_equal(s9, s10));
        }
        void testCase2()
        {
            tsSL<tinystl::string> s1;
            assert(s1.empty());
            assert(s1.size() == 0);
            
            tinystl::string arr[] = {"123", "213", "312", "1231", "3231"};
            tsSL<tinystl::string> s2(std::begin(arr), std::end(arr)); 
            tsSL<tinystl::string> s3(std::begin(arr), std::end(arr));
            assert(!s2.empty());
            assert(s2.size() == 5);
            assert(tinystl::Test::container_equal(s2, s3));
        }
        void testCase3()
        {
            stdS<int> st;
            tsSL<int> sl;
            tinystl::vector<int> v;
            std::random_device rd;
            for(int i = 0; i < 500; ++i)
            {
                auto r = rd() % 65536;
                if(std::find(v.begin(), v.end(), r) != v.end())
                    continue;
                v.push_back(r);
                st.insert(r);
                auto it = sl.insert_unique(r);
                assert(*it == r);
                assert(tinystl::Test::container_equal(st, sl));
            }
            while(!v.empty())
            {
                int idx = rd() % v.size();
                auto r = v[idx];
                st.erase(r);
                auto n = sl.erase(r);
                assert(n == 1);
                v.erase(v.begin() + idx);
                assert(tinystl::Test::container_equal(st, sl));
            }
        }
        void testCase4()
        {
            stdS<double> st;
            tsSL<double> sl;
            std::random_device rd;
            for(int i = 0; i != 100; ++i)
            {
                auto r = rd();
                st.insert(r);
                sl.insert_unique(r);
            }
            auto it1 = st.rbegin();
            auto it2 = sl.rbegin();
            while(it1 != st.rend() && it2 != sl.rend())
            {
                assert(*it1 == *it2);
                ++it1;
                ++it2;
            }
            assert(it1 == st.rend() && it2 == sl.rend());
        }
        void testCase5()
        {
            stdS<int> s1;
            tsSL<int> s2;
            std::random_device rd;
            for(int i = 0; i != 100; ++i)
            {
                auto r = rd() % 65536;
                s1.insert(r);
                s2.insert_unique(r);
            }
            assert(tinystl::Test::container_equal(s1, s2));
            for(int i = 0; i != 50; ++i)
            {
                auto r = rd() % 65536;
                auto p1 = s1.equal_range(r);
                auto p2 = s2.equal_range(r);
                auto it1 = p1.first;
                auto it2 = p2.first;
                while(it1 != p1.second && it2 != p2.second)
                {
                    assert(*it1 == *it2);
                    ++it1;
                    ++it2;
                }
                assert(it1 == p1.second && it2 == p2.second);
                auto it3 = s1.lower_bound(r);
                auto it4 = s2.lower_bound(r);
                assert((it3 == s1.end()) == (it4 == s2.end()));
                assert((it3 != s1.end()) == (it4 != s2.end()));
                if(it3 != s1.end())
                {
                    assert(*it3 == *it4);
                }

                auto it5 = s1.upper_bound(r);
                auto it6 = s2.upper_bound(r);
                assert((it5 == s1.end()) == (it6 == s2.end()));
                assert((it5 != s1.end()) == (it6 != s2.end()));
                if(it5 != s1.end())
                {
                    assert(*it5 == *it6);
                }
            }
        }
        void testCase6()
        {
            stdS<int> s1;
            tsSL<int> s2;
            std::random_device rd;
            for(int i = 0; i != 100; ++i)
            {
                auto r = rd() % 65536;
                s1.insert(r);
                s2.insert_unique(r);
            }
            s1.clear();
            s2.clear();
            assert(s2.empty());
            assert(s2.size() == 0);
            assert(tinystl::Test::container_equal(s1, s2));
        }

        void testCaseAll()
        {
            testCase1();
            testCase2();
            testCase3();
            testCase4();
            testCase5();
            testCase6();
        }
    }
}
