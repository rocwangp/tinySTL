#include "StackTest.h"
#include <list>
#include "../list.h"
#include "../deque.h"

namespace tinystl{
	namespace StackTest{
		void testCase1(){
			stdSt<int> st1;
			tsSt<int> st2;

			for (auto i = 0; i != 200; ++i){
				st1.push(i);
				st2.push(i);
			}
			for (auto i = 0; i != 200; ++i){
				assert(st1.top() == st2.top());
				st1.pop(); 
				st2.pop();
			}
		}
		void testCase2(){
			tsSt<std::string> st;
			assert(st.empty());
			assert(st.size() == 0);

			st.push("one"); 
			st.push("two");
			assert(!st.empty());
			assert(st.size() == 2);
		}
		void testCase3(){
			tsSt<int> st1;
			for (auto i = 0; i != 100; ++i)
				st1.push(i);
			auto st2(st1);
			assert(st1 == st2);
			assert(!(st1 != st2));
		}
		void testCase4(){
			tsSt<int> st1, st2;
			st1.push(1); st1.push(2); st1.push(3);
			st2.push(1); st2.push(2);
			assert(st1.size() == 3 && st2.size() == 2);
			st1.swap(st2);
			assert(st1.size() == 2 && st2.size() == 3);
			/* tinystl::swap(st1, st2); */
			/* assert(st1.size() == 3 && st2.size() == 2); */
		}
        void testCase5()
        {
            std::stack<std::string, std::deque<std::string>> st1;
            tinystl::stack<std::string, tinystl::deque<std::string>> st2;
            for(int i = 0; i != 500; ++i)
            {
                st1.push(std::to_string(i));
                st2.push(std::to_string(i));
            }
            assert(st2 == st1);
        }
         void testCase6()
        {
            std::stack<std::string, std::list<std::string>> st1;
            tinystl::stack<std::string, tinystl::list<std::string>> st2;
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

		void testCase7()
		{
			assert(TestItem::getCount() == 0);
			{
                tsSt<TestItem> st;
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
		}
	}
}

