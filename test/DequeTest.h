#pragma once
#include "TestUtil.h"

#include "../deque.h"
#include <deque>

#include <cassert>
#include <string>

namespace tinystl{
	namespace DequeTest{
		template<class T>
		using stdDQ = std::deque < T > ;
		template<class T>
		using tsDQ = tinystl::deque < T > ;
        template <class T, class Container1, class Container2>
            bool operator==(const tinystl::deque<T, Container1>& lhs,
                            const std::deque<T, Container2>& rhs)
            {
                auto ltmp(lhs);
                auto rtmp(rhs);
                while(!ltmp.empty() || !rtmp.empty())
                {
                    if(ltmp.front() != rtmp.front())
                        return false;
                    ltmp.pop();
                    rtmp.pop();
                }
                return ltmp.empty() && rtmp.empty();
            }
		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();
		void testCase5();
		void testCase6();
        void testCase7();
        void testCase8();

		void testAllCases();
	}
}


