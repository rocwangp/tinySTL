#include <iostream>
#include "DequeTest.h"

namespace tinystl{
	namespace DequeTest{
		void testCase1(){
			stdDQ<int> dq1(20, 0);
			tsDQ<int> dq2(20, 0);
			assert(tinystl::Test::container_equal(dq1, dq2));

			int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			stdDQ<int> dq3(std::begin(arr), std::end(arr));
			tsDQ<int> dq4(std::begin(arr), std::end(arr));
			assert(tinystl::Test::container_equal(dq3, dq4));

			auto dq5(dq1);
			auto dq6(dq2);
			assert(tinystl::Test::container_equal(dq5, dq6));

			auto dq7 = dq3;
			auto dq8 = dq4;
			assert(tinystl::Test::container_equal(dq7, dq8));

			auto dq9 = std::move(dq7);
			auto dq10 = std::move(dq8);
			assert(tinystl::Test::container_equal(dq9, dq10));

            stdDQ<int> dq11{1, 2, 3, 4, 5, 6};
            tsDQ<int> dq12{1, 2, 3, 4, 5, 6};
			assert(tinystl::Test::container_equal(dq11, dq12));
		}
		void testCase2(){
			tsDQ<int> dq1;
			assert(dq1.empty());
			assert(dq1.size() == 0);

			tsDQ<int> dq2(10, 0);
			assert(!dq2.empty());
			assert(dq2.size() == 10);
		}
		void testCase3(){
			stdDQ<std::string> dq1(10, "10");
			tsDQ<std::string> dq2(10, "10");

			dq1[0] = "0"; dq1[9] = "9";
			dq2[0] = "0"; dq2[9] = "9";

			assert(dq1.front() == dq2.front());
			assert(dq1.back() == dq2.back());
			assert(tinystl::Test::container_equal(dq1, dq2));
		}
		void testCase4(){
			stdDQ<int> dq1;
			tsDQ<int> dq2;

			for (auto i = 0; i != 20; ++i){
				dq1.push_back(i);
				dq2.push_back(i);
			}
			assert(tinystl::Test::container_equal(dq1, dq2));

			for (auto i = 10; i != 20; ++i){
				dq1.push_front(i);
				dq2.push_front(i);
			}
			assert(tinystl::Test::container_equal(dq1, dq2));

			for (auto i = 0; i != 5; ++i){
				dq1.pop_back();
				dq2.pop_back();
			}
			assert(tinystl::Test::container_equal(dq1, dq2));

			for (auto i = 0; i != 5; ++i){
				dq1.pop_front();
				dq2.pop_front();
			}
			assert(tinystl::Test::container_equal(dq1, dq2));
		}
		void testCase5(){
			int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			tsDQ<int> foo(arr, arr + 3), bar(arr + 3, arr + 10);

			assert(foo.size() == 3 && bar.size() == 7);
			foo.swap(bar);
			assert(foo.size() == 7 && bar.size() == 3);
			/* tinystl::swap(foo, bar); */
			/* assert(foo.size() == 3 && bar.size() == 7); */
		}
		void testCase6(){
			int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			tsDQ<int> foo1(arr, arr + 3), bar(arr + 3, arr + 10);

			assert(foo1 != bar);
			auto foo2 = bar;
			assert(foo2 == bar);
		}
        void testCase7()
        {
            stdDQ<std::string> dq1(100, "abc");
            tsDQ<std::string> dq2(100, "abc");
			assert(tinystl::Test::container_equal(dq1, dq2));

            dq1.clear();
            dq2.clear();
			assert(tinystl::Test::container_equal(dq1, dq2));
            assert(dq1.size() == 0 && dq2.size() == 0);
        }
        void testCase8()
        {
            stdDQ<std::string> dq1;
            tsDQ<std::string> dq2;
            for(double i = 0; i < 100; ++i)
            {
                dq1.push_back(std::to_string(i));
                dq1.push_front(std::to_string(i - 50));
                dq2.push_back(std::to_string(i));
                dq2.push_front(std::to_string(i - 50));
            }
         
			assert(tinystl::Test::container_equal(dq1, dq2));
            assert(dq1[10] == dq2[10]);
            assert(dq1.at(20) == dq2.at(20));

            dq1[30] = std::to_string(3.14);
            dq2[30] = std::to_string(3.14);
            assert(dq1[30] == dq2[30]);
        }


		void testAllCases(){
			testCase1();
			testCase2();
			testCase3();
			testCase4();
			testCase5();
			testCase6();
            testCase7();
            testCase8();
		}
	}
}

