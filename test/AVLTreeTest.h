#pragma once
#include "TestUtil.h"

#include "../avl_tree.h"

#include <algorithm>
#include <cassert>
#include <random>
#include <string>
#include <vector>

namespace tinystl{
	namespace AVLTreeTest{
		template<class T>
		using tsAVL = tinystl::avl_tree < T > ;

		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();
        void testCase5();


		void testAllCases();
	}
}

