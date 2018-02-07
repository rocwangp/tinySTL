#pragma once

#include "../vector.h"
#include "../numeric.h"
#include "../algorithm.h"

#include <thread>
#include <future>

namespace tinystl
{
    namespace parallel 
    {
        namespace
        {
            template <class Distance>
            auto getThreadNums(Distance count, int minCalNums = 20)
            {
                auto avaThreadNums = std::thread::hardware_concurrency();
                auto maxThreadNums = ((count + (minCalNums - 1)) & (~(minCalNums - 1))) / minCalNums;
                return avaThreadNums == 0 ? maxThreadNums : tinystl::min(static_cast<int>(avaThreadNums), static_cast<int>(maxThreadNums));
            }
        }

        template <class InputIt, class T>
        T accumulate(InputIt first, InputIt last, T init)
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
                threads.emplace_back([front, back, &results, i, init] { results[i] = tinystl::accumulate(front, back, init); });
                front = back;
            }
            for(auto& th : threads)
                th.join();
            return tinystl::accumulate(results.begin(), results.end(), init);
        }

    
        template <class InputIt, class UnaryFunction>
        UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f)
        {
            auto count = tinystl::distance(first, last);
            if(!count)  return f;
            if(count <= 20)
            {
                tinystl::for_each(first, last, f);
            }
            else
            {
                auto middle = first;
                tinystl::advance(middle, count / 2);
                std::async(tinystl::parallel::for_each<InputIt, UnaryFunction>, first, middle, f);
                tinystl::for_each(first, middle, f);
            }
            return f;
        }

        /* template <class InputIt, class T> */
        /* InputIt find(InputIt first, InputIt last, const T& value) */
        /* { */
        /*     auto count = tinystl::distance(first, last); */
        /*     int threadNums = getThreadNums(count); */
        /*     std::cout << threadNums << std::endl; */
        /*     int blockSize = count / threadNums; */
        /*     std::promise<InputIt> result; */
        /*     std::atomic<bool> done(false); */
        /*     auto front = first; */
        /*     { */
        /*         tinystl::vector<std::thread> threads; */
        /*         for(int i = 0; i < threadNums; ++i) */
        /*         { */
        /*             auto back = front; */
        /*             if(i == threadNums - 1) */
        /*                 back = last; */
        /*             else */
        /*                 tinystl::advance(back, blockSize); */
        /*             threads.emplace_back( */
        /*                             [front, back, &value, &done, &result] */
        /*                             { */
        /*                                 for(auto it = front; it != back && !done; ++it) */
        /*                                 { */
        /*                                     if(*it == value) */
        /*                                     { */
        /*                                         done.store(true); */
        /*                                         result.set_value(it); */
        /*                                         return; */
        /*                                     } */
        /*                                 } */
        /*                             } */
        /*                         ); */
        /*             front = back; */
        /*         } */
        /*         for(auto& th : threads) */
        /*             th.join(); */
        /*     } */
        /*     return done ? result.get_future().get() : last; */
        /* } */


        template <class InputIt, class T>
        InputIt find(InputIt first, InputIt last, const T& value)
        {
            auto count = tinystl::distance(first, last);
            if(count <= 10000)
            {
                return tinystl::find(first, last, value);
            }
            else
            {
                auto middle = first;
                tinystl::advance(middle, count / 2);
                auto asyncResult = std::async(find<InputIt, T>, middle, last, std::cref(value));
                auto rightResult = tinystl::parallel::find(first, middle, std::cref(value));
                return rightResult == middle ? asyncResult.get() : rightResult;
            }
        }
    }
}
