#include "SetTest.h"

namespace tinystl
{
    namespace SetTest
    {
        void testCase1()
        {
            stdS<tinystl::string> s1;
            tsS<tinystl::string> s2;
            assert(tinystl::Test::container_equal(s1, s2));

            tinystl::string arr[] = {"abc", "bac", "cab", "cba", "bca", "acb"};
            stdS<tinystl::string> s3(std::begin(arr), std::end(arr));
            tsS<tinystl::string> s4(std::begin(arr), std::end(arr));
            assert(tinystl::Test::container_equal(s3, s4));

            auto s5(s3);
            auto s6(s4);
            assert(tinystl::Test::container_equal(s5, s6));

            auto s7 = s5;
            auto s8 = s6;
            assert(tinystl::Test::container_equal(s7, s8));

            auto s9(std::move(s7));
            auto s10(std::move(s8));
            assert(tinystl::Test::container_equal(s9, s10));
            assert(tinystl::Test::container_equal(s7, s8));

            s5 = s9;
            s6 = s10;
            assert(tinystl::Test::container_equal(s5, s6));

            s1 = std::move(s5);
            s2 = std::move(s6);
            assert(tinystl::Test::container_equal(s5, s6));
            assert(tinystl::Test::container_equal(s1, s2));
        }
        void testCase2()
        {
            tsS<int> s1;
            assert(s1.empty());
            assert(s1.size() == 0);

            int arr[] = {1, 2, 5, 3, 2, 9, 11, 9};
            tsS<int> s2(std::begin(arr), std::end(arr)); 
            assert(!s2.empty());
            assert(s2.size() == 6);
        }
        void testCase3()
        {
            int arr[] = {1, 2, 5, 3, 2, 9, 11, 9};
            stdS<int> s1(std::begin(arr), std::end(arr));
            tsS<int> s2(std::begin(arr), std::end(arr)); 
            assert(tinystl::Test::container_equal(s1, s2));
            s1.clear();
            s2.clear();
            assert(tinystl::Test::container_equal(s1, s2));
            assert(s2.empty());
            assert(s2.size() == 0);
        }
        void testCase4()
        {
            stdS<double> s1;
            tsS<double> s2;
            std::random_device rd;
            for(int i = 0; i != 100; ++i)
            {
                auto r = rd();
                auto p1 = s1.insert(r);
                auto p2 = s2.insert(r);
                assert((p1.first == s1.end()) == (p2.first == s2.end()));
                assert((p1.first != s1.end()) == (p2.first != s2.end()));
                assert(p1.second == p2.second);
                if(p1.first != s1.end())
                    assert(*p1.first == *p2.first);
            }
        }
        void testCase5()
        {
            stdS<double> s1;
            tsS<double> s2;
            tinystl::vector<double> v;
            std::random_device rd;
            for(int i = 0; i != 100; ++i)
            {
                auto r = rd();
                s1.insert(r);
                s2.insert(r);
                if(std::find(v.begin(), v.end(), r) == v.end())
                    v.push_back(r);
            }

            while(!v.empty())
            {
                int idx = rd() % (v.size());
                auto r = v[idx];
                auto n1 = s1.erase(r);
                auto n2 = s2.erase(r);
                assert(n1 == n2);
                assert(tinystl::Test::container_equal(s1, s2));
                v.erase(v.begin() + idx);
            }
        }
        void testCase6()
        {
            stdS<double> s1;
            tsS<double> s2;
            std::random_device rd;
            for(int i = 0; i != 100; ++i)
            {
                auto r = rd();
                s1.insert(r);
                s2.insert(r);
            }
            assert(tinystl::Test::container_equal(s1, s2));
            stdS<double> s3;
            tsS<double> s4;
            s3.swap(s1);
            s4.swap(s2);
            assert(s2.empty());
            assert(!s4.empty());
            assert(tinystl::Test::container_equal(s1, s2));
            assert(tinystl::Test::container_equal(s3, s4));

            std::swap(s1, s3);
            std::swap(s2, s4);

            assert(tinystl::Test::container_equal(s1, s2));
            assert(tinystl::Test::container_equal(s3, s4));
        }
        void testCase7()
        {
            stdS<double> s1;
            tsS<double> s2;
            tinystl::vector<double> v;
            std::random_device rd;
            for(int i = 0; i != 100; ++i)
            {
                auto r = rd();
                s1.insert(r);
                s2.insert(r);
                if(std::find(v.begin(), v.end(), r) == v.end())
                    v.push_back(r);
            }
            const tsS<double> s3 = s2;

            while(!v.empty())
            {
                auto idx = rd() % v.size();
                auto r = v[idx];
                v.erase(v.begin() + idx);
                stdS<double>::const_iterator it1 = s1.find(r);
                tsS<double>::const_iterator it2 = s2.find(r);
                tsS<double>::const_iterator it3 = s3.find(r);
                assert(*it1 == *it2);
                assert(*it1 == *it3);
                assert(s1.count(r) == s2.count(r));
                assert(s1.count(r) == s3.count(r));
            }
        }
        void testCaseAll()
        {
            testCase1();
            testCase2();
            testCase3();
            testCase4();
            testCase5();
            testCase6();
            testCase7();
        }
    }
}
