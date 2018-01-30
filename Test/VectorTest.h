#include "../vector.h"
#include "TestUtil.h"
#include <vector>

#include <array>
#include <cassert>
#include<iostream>
#include <iterator>
#include <string>

namespace tinystl{
	namespace VectorTest{
		template<class T>
		using stdVec = std::vector < T >;

		template<class T>
		using tsVec = tinystl::vector < T >;

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

		void testAllCases();
	}
}
