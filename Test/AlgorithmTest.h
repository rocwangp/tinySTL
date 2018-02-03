#pragma once
#include "TestUtil.h"

#include "../algorithm.h"
#include <algorithm>

#include <array>
#include <cctype>
#include <cstring>
#include <cassert>
#include <functional>
#include <list>
#include <random>
#include <vector>

/* #include "../BinarySearchTree.h" */
#include "../avl_tree.h"
#include "../list.h"
#include "../vector.h"
#include "../utility.h"


namespace tinystl{
	namespace AlgorithmTest{
		void testFill();
		void testFillN();
		void testMinMax();
		void testHeapAlgorithm();
		void testIsHeap();
		void testAllOf();
		void testNoneOf();
		void testAnyOf();
		void testForEach();
		void testFind();
		void testFindEnd();
		void testFindFirstOf();
		void testAdjacentFind();
		void testCount();
		void testMismatch();
		void testEqual();
		void testIsPermutation();
		void testSearch();
		void testAdvance();
		void testSort();
		void testGenerate();
		void testDistance();
		void testCopy();

		void testAllCases();
	}
}

