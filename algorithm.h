#pragma once
#include <algorithm>
#include <iostream>
namespace tinystl
{

/* template <class RandomIterator> */
/* void print(RandomIterator first, RandomIterator last) */
/* { */
/*     for(auto it = first; it != last; ++it) */
/*         std::cout << *it << " "; */
/*     std::cout << std::endl; */
/* } */
template <class RandomIterator, class Compare>
void make_heap(RandomIterator first,
               RandomIterator last,
               Compare comp)
{
    if(first == last)   return;
    auto it = first + (last - first) / 2 - 1;
    for(; it >= first; --it)
    {
        auto value = *(it);
        RandomIterator currentIt = it ;
        RandomIterator childrenIt = currentIt + (currentIt - first) + 1;
        while(childrenIt < last)
        {
            if(childrenIt + 1 < last && comp(*childrenIt, *(childrenIt + 1)))
                ++childrenIt;
            if(comp(*childrenIt, value))
                break;
            *currentIt = *childrenIt;
            currentIt = childrenIt;
            childrenIt = currentIt + (currentIt - first) + 1;
        }
        *currentIt = value;
    }
}

template <class RandomIterator, class Compare>
void push_heap(RandomIterator first, RandomIterator last, Compare comp)
{
    auto value = *(last - 1);
    RandomIterator currentIt = last - 1;
    RandomIterator parentIt = first + (currentIt - first + 1) / 2 - 1;
    while(currentIt > first)
    {
        if(comp(value, *parentIt))
            break;
        *currentIt = *parentIt;
        currentIt = parentIt;
        parentIt = first + (currentIt - first + 1) / 2 - 1;
    }
    *currentIt = value;
}

template <class RandomIterator, class Compare>
void pop_heap(RandomIterator first, RandomIterator last, Compare comp)
{
    std::swap(*first, *(last - 1));
    auto value = *first;
    RandomIterator currentIt = first;
    RandomIterator childrenIt = currentIt + (currentIt - first) + 1;
    while(childrenIt < last - 1)
    {
        if(childrenIt + 1 < last - 1 && comp(*childrenIt, *(childrenIt + 1)))
            ++childrenIt;
        if(comp(*childrenIt, value))
            break;
        *currentIt = *childrenIt;
        currentIt = childrenIt;
        childrenIt = currentIt + (currentIt - first) + 1;
    }
    *currentIt = value;
}
}
