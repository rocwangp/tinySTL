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


