#include "ParallerAccumulateTest.h"


namespace tinystl
{
    namespace parallel 
    {
        namespace ParallerAccumulateTest
        {
            using namespace tinystl::profiler;
            void testCase1()
            {
                tinystl::vector<long long int> v(100000000);
                std::random_device rd;
                tinystl::generate(v.begin(), v.end(), [&rd]() { return rd() % 1000; });
                ProfilerInstance::start(); 
                auto result = tinystl::parallel::accumulate(v.begin(), v.end(), 0);
                ProfilerInstance::finish(); 
                ProfilerInstance::dumpDuringTime(); 
                std::cout << result << std::endl;

                ProfilerInstance::start(); 
                result =  std::accumulate(v.begin(), v.end(), 0);
                ProfilerInstance::finish(); 
                ProfilerInstance::dumpDuringTime(); 
                std::cout << result << std::endl;

            }

            void testCase2()
            {
                tinystl::vector<int> v(10);
                std::random_device rd;
                tinystl::generate(v.begin(), v.end(), [&rd] { return rd() % 1000;} );
                tinystl::vector<int> v2 = v;
                tinystl::parallel::for_each(v.begin(), v.end(), [](auto& value) { value *= value; });
                std::for_each(v2.begin(), v2.end(), [](auto& value) { value *= value; });
                assert(v == v2);
            }

            void testCase3()
            {
                tinystl::vector<long long int> v(10000000);
                long long int value = 9000000;
                long long int i = 0;
                tinystl::generate(v.begin(), v.end(), [&i] { return ++i; });
                ProfilerInstance::start(); 
                auto r1 = tinystl::parallel::find(v.begin(), v.end(), value);
                ProfilerInstance::finish(); 
                ProfilerInstance::dumpDuringTime(); 

                ProfilerInstance::start(); 
                auto r2 = std::find(v.begin(), v.end(), value);
                ProfilerInstance::finish(); 
                ProfilerInstance::dumpDuringTime(); 

                assert(r1 == r2);
            }
            void testAllCase()
            {
                /* testCase1(); */
                /* testCase2(); */
                testCase3();
            }
        }
    }
}

