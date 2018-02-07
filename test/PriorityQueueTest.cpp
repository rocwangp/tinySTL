#include <iostream>
#include "PriorityQueueTest.h"
#include <list>
#include "../list.h"

namespace tinystl{
	namespace PriorityQueueTest{
		void testCase1(){
			int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, -1, -2, -3 };
			stdPQ<int> pq1(std::begin(arr), std::end(arr));
			tsPQ<int> pq2(std::begin(arr), std::end(arr));
			
			while (!pq1.empty() && !pq2.empty()){
				assert(pq1.top() == pq2.top());
				pq1.pop(); pq2.pop();
			}
			assert(pq1.empty() && pq2.empty());
		}
		void testCase2(){
			tsPQ<std::string> pq;
			assert(pq.empty());

			pq.push("zxh");
			assert(!pq.empty());
		}
		void testCase3(){
			tsPQ<int> pq;
			auto i = 1;
			for (; i != 10; ++i){
				pq.push(i);
				assert(pq.size() == i);
			}
			for (i = pq.size(); i != 0; --i){
				pq.pop();
				assert(pq.size() == (i - 1));
			}
		}
		void testCase4(){
			stdPQ<int> pq1;
			tsPQ<int> pq2;

			pq1.push(30);
			pq1.push(100);
			pq1.push(25);
			pq1.push(40);

			pq2.push(30);
			pq2.push(100);
			pq2.push(25);
			pq2.push(40);

			while (!pq1.empty() && !pq2.empty()){
				assert(pq1.top() == pq2.top());
				pq1.pop(); 
				pq2.pop();
			}
		}
		void testCase5(){
			tsPQ<int> foo, bar;
			foo.push(15); foo.push(30); foo.push(10);
			bar.push(101); bar.push(202);

			assert(foo.size() == 3 && bar.size() == 2);
			foo.swap(bar);
			assert(foo.size() == 2 && bar.size() == 3);

			/* tinystl::swap(foo, bar); */
			/* assert(foo.size() == 3 && bar.size() == 2); */
		}
        void testCase6()
        {
            std::priority_queue<int, vector<int>, std::greater<int>> pq1;
            tinystl::priority_queue<int, vector<int>, std::greater<int>> pq2;
            for(int i = 0; i < 100; ++i)
            {
                pq1.push(i);
                pq1.push(i - 50);
                pq2.push(i);
                pq2.push(i - 50);
            }
            while(!pq1.empty() && !pq2.empty())
            {
                assert(pq1.top() == pq2.top());
                pq1.pop();
                pq2.pop();
            }
            assert(pq1.empty() && pq2.empty());
        }
        
        class TestItem
        {
        public:
            TestItem()
            {
                ++count;
                priCnt_ = count;
            }

            bool operator<(const TestItem& rhs) const
            { return priCnt_ < rhs.priCnt_; }
            bool operator>(const TestItem& rhs) const
            { return priCnt_ > rhs.priCnt_; }
            bool operator==(const TestItem& rhs) const
            { return priCnt_ == rhs.priCnt_; }
        private:
            static int count;
            int priCnt_;
        };
        int TestItem::count = 0;

        void testCase7()
        {
            stdPQ<TestItem> pq1;
            tsPQ<TestItem> pq2;
            
            for(int i = 0; i < 100; ++i)
            {
                TestItem item;
                pq1.push(item);
                pq2.push(item);
            }

            while(!pq1.empty() && !pq2.empty())
            {
                assert(pq1.top() == pq2.top());
                pq1.pop();
                pq2.pop();
            }
            assert(pq1.empty() && pq2.empty());
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

