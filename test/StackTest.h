#pragma once
#include "TestUtil.h"

#include "../stack.h"
#include <stack>

#include <cassert>
#include <string>

namespace tinystl{
	namespace StackTest{
		template<class T>
		using stdSt = std::stack < T > ;
		template<class T>
		using tsSt = tinystl::stack < T > ;

        template <class T, class Container1, class Container2>
            bool operator==(const tinystl::stack<T, Container1>& lhs,
                            const std::stack<T, Container2>& rhs)
            {
                auto ltmp(lhs);
                auto rtmp(rhs);
                while(!ltmp.empty() || !rtmp.empty())
                {
                    if(ltmp.top() != rtmp.top())
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
		void testAllCases();
	}
}

