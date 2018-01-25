#pragma once
#include "TestUtil.h"

#include "../priority_queue.h"
#include <queue>

#include <algorithm>
#include <cassert>
#include <string>

namespace tinystl{
	namespace PriorityQueueTest{
		template<class T>
		using stdPQ = std::priority_queue < T > ;
		template<class T>
		using tsPQ = tinystl::priority_queue < T > ;

        template <class Container>
            void print(Container cont)
            {
                Container tmp(cont);
                while(!tmp.empty())
                {
                    std::cout << tmp.top() << " ";
                    tmp.pop();
                }
                std::cout << std::endl;
            }
		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();
		void testCase5();
        void testCase6();
        void testCase7();

		void testAllCases();
	}
}



