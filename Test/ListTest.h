#pragma once
#include "TestUtil.h"

#include "../list.h"
#include <list>

#include <cassert>
#include <functional>
#include <string>
#include <random>

namespace tinystl{
	namespace ListTest{
		template<class T>
		using stdL = std::list < T > ;
		template<class T>
		using tsL = tinystl::list < T > ;

		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();
		void testCase5();
		void testCase6();
		void testCase7();
		void testCase8();
		void testCase9();
		void testCase10();
		void testCase11();
		void testCase12();
		void testCase13();
		void testCase14();
		void testCase15();
        void testCase16();
        void testCase17();
        void testCase18();

		void testAllCases();
	}
}


