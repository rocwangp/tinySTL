#pragma once

#include "../unordered_set.h"
#include "TestUtil.h"
#include <vector>

#include <random>
#include <unordered_set>
#include <array>
#include <cassert>
#include <iostream>
#include <iterator>
#include <string>

namespace tinystl{
	namespace UnorderedSetTest{
		template<class T>
		using stdUst = std::unordered_set < T >;

		template<class T>
		using tsUst = tinystl::unordered_set < T >;

		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();
		void testCase5();

		void testAllCases();
	}
}

