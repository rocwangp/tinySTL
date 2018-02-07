#include "QueueTest.h"
#include <iostream>
#include <list>
#include "../list.h"
#include "../deque.h"
namespace tinystl{
	namespace QueueTest{
		void testCase1(){
			stdQ<int> q1;
			tsQ<int> q2;

			for (auto i = 0; i != 100; ++i){
				q1.push(i);
				q2.push(i);
			}
			for (auto i = 0; i != 100; ++i){
				assert(q1.front() == q2.front());
				q1.pop();
				q2.pop();
			}
		}
		void testCase2(){
			tsQ<int> q1;
			for (auto i = 0; i != 100; ++i)
				q1.push(i);
			auto q2(q1);
			assert(q1 == q2);
			assert(!(q1 != q2));
		}
		void testCase3(){
			tsQ<int> q;
			assert(q.empty());
			assert(q.size() == 0);
			
			q.push(10);
			q.push(11);
			assert(!q.empty());
			assert(q.size() == 2);
		}
		void testCase4(){
			tsQ<std::string> q;
			q.push("front");
			q.push("back");
			
			assert(q.front() == "front");
			assert(q.back() == "back");
		}
		void testCase5(){
			tsQ<int> q1, q2;

			q1.push(1); q1.push(2); q1.push(3);
			q2.push(1); q2.push(2);

			assert(q1.size() == 3 && q2.size() == 2);
			q1.swap(q2);
			assert(q1.size() == 2 && q2.size() == 3);
			/* tinystl::swap(q1, q2); */
			/* assert(q1.size() == 3 && q2.size() == 2); */
		}
        void testCase6()
        {
            std::queue<std::string, std::deque<std::string>> st1;
            tinystl::queue<std::string, tinystl::deque<std::string>> st2;
            for(int i = 0; i != 500; ++i)
            {
                st1.push(std::to_string(i));
                st2.push(std::to_string(i));
            }
            assert(st2 == st1);
        }
         void testCase7()
        {
            std::queue<std::string, std::list<std::string>> st1;
            tinystl::queue<std::string, tinystl::list<std::string>> st2;
            for(int i = 0; i != 500; ++i)
            {
                st1.push(std::to_string(i));
                st2.push(std::to_string(i));
            }
            for(int i = 0; i < 200; ++i)
            {
                st1.pop();
                st2.pop();
            }
            assert(st2 == st1);
        }
        class TestItem
		{
		public:
			TestItem()
			{
				++count;
			}
			TestItem(const TestItem & )
			{
				++count;
			}

			virtual ~TestItem()
			{
				--count;
			}

			static int getCount()
			{
				return count;
			}
		private:
			static int count;
		};
		int TestItem::count = 0;

		void testCase8()
		{
			assert(TestItem::getCount() == 0);
			{
                tsQ<TestItem> st;
                for(int i = 0; i < 100; ++i)
                    st.push(TestItem());
                for(int i = 0; i < 20; ++i)
                    st.pop();
			}
			assert(TestItem::getCount() == 0);

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

