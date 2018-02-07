#pragma once
#include "TestUtil.h"

#include "../queue.h"
#include <queue>

#include <cassert>
#include <string>

namespace tinystl{
	namespace QueueTest{
		template<class T>
		using stdQ = std::queue < T > ;
		template<class T>
		using tsQ = tinystl::queue < T > ;

	
        template <class T, class Container1, class Container2>
            bool operator==(const tinystl::queue<T, Container1>& lhs,
                            const std::queue<T, Container2>& rhs)
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

