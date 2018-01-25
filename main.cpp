#include <iostream>

/* #include "Profiler\Profiler.h" */

/* #include "Test\AlgorithmTest.h" */
/* #include "Test\AVLTreeTest.h" */
/* #include "Test\BitmapTest.h" */
/* #include "Test\BinarySearchTreeTest.h" */
/* #include "Test\CircularBufferTest.h" */
/* #include "Test\COWPtrTest.h" */
#include "Test/DequeTest.h"
/* #include "Test\GraphTest.h" */
#include "Test/ListTest.h"
/* #include "Test\PairTest.h" */
#include "Test/PriorityQueueTest.h"
#include "Test/QueueTest.h"
/* #include "Test\RefTest.h" */
/* #include "Test\SharedPtrTest.h" */
#include "Test/StackTest.h"
/* #include "Test\StringTest.h" */
/* #include "Test\SuffixArrayTest.h" */
/* #include "Test\TrieTreeTest.h" */
/* #include "Test\TypeTraitsTest.h" */
/* #include "Test\UFSetTest.h" */
/* #include "Test\UniquePtrTest.h" */
/* #include "Test\Unordered_setTest.h" */
#include "Test/VectorTest.h"

/* using namespace tinystl::Profiler; */

int main(){
	/* tinystl::AlgorithmTest::testAllCases(); */
	/* tinystl::AVLTreeTest::testAllCases(); */
	/* tinystl::BitmapTest::testAllCases(); */
	/* tinystl::BinarySearchTreeTest::testAllCases(); */
	/* tinystl::CircularBufferTest::testAllCases(); */
	/* tinystl::COWPtrTest::testAllCases(); */
	tinystl::StackTest::testAllCases();
	tinystl::DequeTest::testAllCases();
	tinystl::ListTest::testAllCases();
	/* tinystl::GraphTest::testAllCases(); */
	/* tinystl::PairTest::testAllCases(); */
	tinystl::PriorityQueueTest::testAllCases();
	tinystl::QueueTest::testAllCases();
	/* tinystl::RefTest::testAllCases(); */
	/* tinystl::SharedPtrTest::testAllCases(); */
	/* tinystl::StringTest::testAllCases(); */
	/* tinystl::SuffixArrayTest::testAllCases(); */
	/* tinystl::TrieTreeTest::testAllCases(); */
	/* tinystl::TypeTraitsTest::testAllCases(); */
	/* tinystl::UFSetTest::testAllCases(); */
	/* tinystl::UniquePtrTest::testAllCases(); */
	/* tinystl::Unordered_setTest::testAllCases(); */
	tinystl::VectorTest::testAllCases();
	 
	return 0;
}

