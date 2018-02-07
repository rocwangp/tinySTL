#include "MapTest.h"

namespace tinystl
{
    namespace MapTest
    {
        void testCase1()
        {
            stdM<int, tinystl::string> m1;
            tsM<int, tinystl::string> m2;
            assert(tinystl::Test::container_equal(m1, m2));

            std::pair<int, tinystl::string> arr[] = {{5, "abc"}, {2, "bca"}, {10, "cba"}, {1, "acb"}, {3, "bac"}};
            stdM<int, tinystl::string> m3(std::begin(arr), std::end(arr));
            tsM<int, tinystl::string> m4(std::begin(arr), std::end(arr));
            assert(tinystl::Test::container_equal(m3, m4));

            auto m5(m3);
            auto m6(m4);
            assert(tinystl::Test::container_equal(m5, m6));

            auto m7(std::move(m5));
            auto m8(std::move(m6));
            assert(tinystl::Test::container_equal(m7, m8));
            assert(tinystl::Test::container_equal(m5, m6));

            m1 = m7;
            m2 = m8;
            assert(tinystl::Test::container_equal(m1, m2));
        }
        void testCase2()
        {
            stdM<int, tinystl::string> m1;
            tsM<int, tinystl::string> m2;
            std::random_device rd;
            for(int i = 0; i < 100; ++i)
            {
                int  r = rd() % 65536;
                m1.insert(std::make_pair(r, tinystl::to_string(r)));
                m2.insert(std::make_pair(r, tinystl::to_string(r)));
                assert(tinystl::Test::container_equal(m1, m2));
            }
        }
        void testCase3()
        {
            stdM<int, tinystl::string> m1;
            tsM<int, tinystl::string> m2;
            tinystl::vector<int> v;
            std::random_device rd;
            for(int i = 0; i < 100; ++i)
            {
                auto r = rd() % 65536;
                if(std::find(v.begin(), v.end(), r) == v.end())
                    v.push_back(r);
                auto p1 = m1.insert(std::make_pair(r, tinystl::to_string(r)));
                auto p2 = m2.insert(std::make_pair(r, tinystl::to_string(r)));
                assert(p1.second == p2.second);
                if(p1.second)
                    assert(*p1.first == *p2.first);
                assert(tinystl::Test::container_equal(m1, m2));
            }
            while(!v.empty())
            {
                auto idx = rd() % v.size();
                auto r = v[idx];
                auto n1 = m1.erase(r);
                auto n2 = m2.erase(r);
                assert(n1 == n2);
                v.erase(v.begin() + idx);
            }
        }
        void testCase4()
        {
            stdM<int, tinystl::string> m1;
            tsM<int, tinystl::string> m2;
            std::random_device rd;
            for(int i = 0; i < 100; ++i)
            {
                auto r = rd() % 100;
                m1[r] = tinystl::to_string(r);
                m2[r] = tinystl::to_string(r);
                assert(m1.at(r) == m2.at(r));
                assert(tinystl::Test::container_equal(m1, m2));
                assert(m1[r] == m2[r]);
            }
        }
        void testCase5()
        {
            tsM<int, int> m1;
            assert(m1.empty());
            assert(m1.size() == 0);

            m1[0] = 1;
            assert(!m1.empty());
            assert(m1.size() == 1);

            m1.clear();
            assert(m1.empty());
            assert(m1.size() == 0);
        }
        class TestItem
        {
        public:
            TestItem()
            {
                ++count;
                n = count;
            }

            virtual ~TestItem()
            {
                --count;
            }

            bool operator==(const TestItem& rhs)
            {
                return n == rhs.n;
            }
        private:
            static int count;
            int n;
        };
        int TestItem::count = 0;

        void testCase6()
        {
            stdM<int, tinystl::string> m1;
            tsM<int, tinystl::string> m2;
            std::random_device rd;
            for(int i = 0; i < 100; ++i)
            {
                int r = rd() % 65536;
                m1[r] = tinystl::to_string(r);
                m2[r] = tinystl::to_string(r);
                assert(tinystl::Test::container_equal(m1, m2));
            }
            auto it1 = m1.rbegin();
            auto it2 = m2.rbegin();
            while(it1 != m1.rend() && it2 != m2.rend())
            {
                assert(*it1 == *it2);
                ++it1;
                ++it2;
            }
            assert(it1 == m1.rend() && it2 == m2.rend());
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
