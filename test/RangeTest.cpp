#include "RangeTest.h"

namespace tinystl{
	namespace RangeTest {
        void testCase1() {
            std::cout << "Range(2, 10)" << std::endl;
            for(auto i : range(2, 10)) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            std::cout << "Range(5, 10, 2)" << std::endl;
            for(auto i : range(5, 10, 2)) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            std::cout << "Range(15)" << std::endl;
            for(auto i : range(15)) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            std::cout << "Range(10.5, 15.5)" << std::endl;
            for(auto i : range(10.5, 15.5)) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            std::cout << "Range(35, 27, -3)" << std::endl;
            for(auto i : range(35, 27, -3)) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            std::cout << "Range(7, 8, 0.1)" << std::endl;
            for(auto i : range(7, 8, 0.1)) {
                std::cout << i << " ";
            }
            std::cout << std::endl;


            std::cout << "Range('a', 'z')" << std::endl;
            for(auto i : range('a', 'z')) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }

        void testAllCases() {
            testCase1();
        }

    }
}



