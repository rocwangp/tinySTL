#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
#include <functional>
#include <random>

#include "../vector.h"
#include "../algorithm.h"
#include "../numeric.h"

namespace tinystl
{
    namespace net
    {
        template <class Distance>
        auto getThreadNums(Distance count)
        {
            auto avaThreadNums = std::thread::hardware_concurrency();
            auto minCalNums = 20;
            auto maxThreadNums = ((count + (minCalNums - 1)) & (~(minCalNums - 1))) / minCalNums;
            return avaThreadNums == 0 ? maxThreadNums : tinystl::min(static_cast<int>(avaThreadNums), static_cast<int>(maxThreadNums));
        }

        template <class InputIt, class T>
        T parallel_accumulate(InputIt first, InputIt last, T init)
        {
            auto count = tinystl::distance(first, last);
            int threadNums = getThreadNums(count);
            int blockSize = count / threadNums;
            tinystl::vector<std::thread> threads;
            tinystl::vector<T> results(threadNums);
            auto front = first;
            for(int i = 0; i < threadNums; ++i)
            {
                auto back = front;
                if(i != threadNums - 1)
                    tinystl::advance(back, blockSize);
                else
                    back = last;
                threads.emplace_back([front, back, &results, i, init] { results[i] = std::accumulate(front, back, init); });
                front = back;
            }
            for(auto& th : threads)
                th.join();
            return tinystl::accumulate(results.begin(), results.end(), init);
        }
    }
}





