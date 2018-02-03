#pragma once

#include "iterator.h"
#include "utility.h"
#include <iostream>

namespace tinystl
{
template <class T>
void swap(T& a, T& b) 
{
   T tmp = std::move(a);
   a = std::move(b);
   b = std::move(tmp);
}

template <class InputIt>
void print(InputIt first, InputIt last)
{
    while(first != last)
        std::cout << *(first++) << " ";
    std::cout << std::endl;
}

/* No Modify */
template <class InputIt, class UnaryPredicate>
bool all_of(InputIt first, InputIt last, UnaryPredicate p)
{
    while(first != last)
    {
        if(!p(*first))
            return false;
        ++first;
    }
    return true;
}

template <class InputIt, class UnaryPredicate>
bool any_of(InputIt first, InputIt last, UnaryPredicate p)
{
    while(first != last)
    {
        if(p(*first))
            return true;
        ++first;
    }
    return false;
}

template <class InputIt, class UnaryPredicate>
bool none_of(InputIt first, InputIt last, UnaryPredicate p)
{
    while(first != last)
    {
        if(p(*first))
            return false;
        ++first;
    }
    return true;
}

template <class InputIt, class UnaryFunction>
UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f)
{
    while(first != last)
    {
        f(*first);
        ++first;
    }
    return f;
}

template <class ForwardIt1, class ForwardIt2, class BinaryPredicate>
ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
                  ForwardIt2 s_first, ForwardIt2 s_last,
                  BinaryPredicate p)
{
    auto count2 = tinystl::distance(s_first, s_last);
    ForwardIt2* it2Vec = new ForwardIt2[count2];
    int i = 0;
    for(auto it = s_first; it != s_last; ++it)
        it2Vec[i++] = it;

    int* prefix = new int[count2];
    for(auto i = 0; i < count2; ++i)
        prefix[i] = 0;
    int lp = 0;
    for(i = 1; i < count2; ++i)
    {
        while(lp > 0 && !p(*it2Vec[i], *it2Vec[lp]))
            lp = prefix[lp - 1];
        if(p(*it2Vec[i], *it2Vec[lp]))
            ++lp;
        prefix[i] = lp;
    }

    auto count1 = tinystl::distance(first, last);
    ForwardIt1* it1Vec = new ForwardIt1[count1];
    i = 0;
    for(auto it = first; it != last; ++it)
        it1Vec[i++] = it;
    
    lp = 0;
    for(i = 0; i < count1; ++i)
    {
        while(lp > 0 && !p(*it1Vec[i], *it2Vec[lp]))
            lp = prefix[lp - 1];
        if(p(*it1Vec[i], *it2Vec[lp]))
            ++lp;
        if(lp == count2)
            break;
    }
    ForwardIt1 it = (lp != count2) ? last : it1Vec[i + 1 - lp];
    delete[] it1Vec;
    delete[] it2Vec;
    delete[] prefix;
    return it;
}


template <class ForwardIt1, class ForwardIt2>
ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
                  ForwardIt2 s_first, ForwardIt2 s_last)
{
    return tinystl::search(first, last, s_first, s_last, 
                  [](const auto& lhs, const auto& rhs){
                        return lhs == rhs;
                  });
}



template <class InputIterator, class T>
InputIterator find(InputIterator first , InputIterator last, const T& value)
{
    while(first != last)
    {
        if(value == *first)
            return first;
        ++first;
    }
    return last;
}



template <class InputIterator, class UnaryPredicate>
InputIterator find_if(InputIterator first , InputIterator last, UnaryPredicate p)
{
    while(first != last)
    {
        if(p(*first))
            return first;
        ++first;
    }
    return last;
}


template <class InputIterator, class UnaryPredicate>
InputIterator find_if_not(InputIterator first , InputIterator last, UnaryPredicate q)
{
    while(first != last)
    {
        if(!q(*first))
            return first;
        ++first;
    }
    return last;
}

template <class ForwardIt1, class ForwardIt2, class BinaryPredicate>
ForwardIt1 find_first_of(ForwardIt1 first, ForwardIt1 last,
                         ForwardIt2 s_first, ForwardIt2 s_last,
                         BinaryPredicate p)
{
    while(first != last)
    {
        ForwardIt2 cur = s_first;
        while(cur != s_last)
        {
            if(p(*first, *cur))
                return first;
            ++cur;
        }
        ++first;
    }
    return last;
}
template <class ForwardIt1, class ForwardIt2>
ForwardIt1 find_first_of(ForwardIt1 first, ForwardIt1 last,
                         ForwardIt2 s_first, ForwardIt2 s_last)
{
    return tinystl::find_first_of(first, last, s_first, s_last, [](const auto& lhs, const auto& rhs){
                            return lhs == rhs;
                         });
}

template <class ForwardIt1, class ForwardIt2, class BinaryPredicate>
ForwardIt1 find_end(ForwardIt1 first, ForwardIt1 last,
                    ForwardIt2 s_first, ForwardIt2 s_last,
                    BinaryPredicate p)
{
    ForwardIt1 result = last;
    while(true)
    {
        ForwardIt1 newResult = tinystl::search(first, last, s_first, s_last, p);
        if(newResult == last)
            return result;
        result = newResult;
        ++first;
    }
}
template <class ForwardIt1, class ForwardIt2>
ForwardIt1 find_end(ForwardIt1 first, ForwardIt1 last,
                    ForwardIt2 s_first, ForwardIt2 s_last)
{
    return tinystl::find_end(first, last, s_first, s_last, [](const auto& lhs, const auto& rhs){
                        return lhs == rhs;
                    });
}


template <class ForwardIt, class BinaryPredicate>
ForwardIt adjacent_find(ForwardIt first, ForwardIt last, BinaryPredicate p)
{
    while(first != last)
    {
        ForwardIt next = first;
        ++next;
        if(next != last && p(*first, *next))
            return first;
        first = next;
    }
    return last;
}
template <class ForwardIt>
ForwardIt adjacent_find(ForwardIt first, ForwardIt last)
{
    return tinystl::adjacent_find(first, last, [](const auto& lhs, const auto& rhs){
                                    return lhs == rhs;
                                  });
}

template <class InputIterator, class UnaryPredicate>
typename tinystl::iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last, UnaryPredicate p)
{
    auto cnt = tinystl::difference_type(first);
    while(first != last)
    {
        if(p(*first))
            ++cnt;
        ++first;
    }
    return cnt;
}

template <class InputIterator, class T>
typename tinystl::iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value)
{
    return tinystl::count_if(first, last, [&value](const auto& v) { return v == value; });
}



template <class InputIt1, class InputIt2, class BinaryPredicate>
tinystl::pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1,
                                           InputIt2 first2, BinaryPredicate p)
{
    while(first1 != last1 && (p(*first1, *first2)))
    {
        ++first1;
        ++first2;
    }
    return tinystl::make_pair(first1, first2);
}

template <class InputIt1, class InputIt2>
tinystl::pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1,
                                           InputIt2 first2)
{
    return tinystl::mismatch(first1, last1, first2, [](const auto& lhs, const auto& rhs){
                        return lhs == rhs;
                    });
}


/* Compare */
template <class InputIt1, class InputIt2, class BinaryPredicate>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p)
{
    while(first1 != last1 && p(*first1, *first2))
    {
        ++first1;
        ++first2;
    }
    return first1 == last1;
}
template <class InputIt1, class InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
    return tinystl::equal(first1, last1, first2, [](const auto& lhs, const auto& rhs){
                            return lhs == rhs;
                          });
}



/* Permutation */
template <class ForwardIt1, class ForwardIt2>
bool is_permutation(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2)
{
    auto count = tinystl::distance(first1, last1);
    int *visited = new int[count];
    for(auto i = 0; i < count; ++i)
        visited[i] = 0;
    while(first1 != last1)
    {
        auto f2 = first2;
        auto i = 0;
        for(i = 0; i < count; ++i)
        {
            if(*f2 == *first1 && visited[i] == 0)
            {
                visited[i] = 1;
                break;
            }
            ++f2;
        }
        if(i == count)
            break;
        ++first1;
    }
    delete[] visited;
    return first1 == last1 ? true : false;
}

/* Sort */
namespace 
{

    template <class RandomIt, class Compare = std::less_equal<typename iterator_traits<RandomIt>::value_type>>
    void enableFirstInThreeValue(RandomIt first, RandomIt last, Compare comp = Compare())
    {
        RandomIt middle = first + (last - first) / 2;
        --last;
        if(!comp(*first, *middle))
            tinystl::swap(*first, *middle);
        if(!comp(*first, *last))
            tinystl::swap(*first, *last);
        if(!comp(*middle, *last))
            tinystl::swap(*middle, *last);
        tinystl::swap(*first, *middle);
    }

    template <class RandomIt, class Compare = std::less_equal<typename iterator_traits<RandomIt>::value_type>>
    void insertSort(RandomIt first, RandomIt last, Compare comp = Compare())
    {
        auto cur = first;
        while(cur != last)
        {
            auto value = *cur;
            auto prev = cur - 1;
            auto next = cur + 1;
            while(cur != first && comp(value, *prev))
            {
                *cur = *prev;
                --prev;
                --cur;
            }
            *cur = value;
            cur = next;
        }
    }

}
template <class RandomIt, class Compare = std::less_equal<typename iterator_traits<RandomIt>::value_type>>
void sort(RandomIt first, RandomIt last, Compare comp = Compare())
{
    if(first == last || first + 1 == last)
        return;
    if(last - first <= 10)
    {
        insertSort(first, last, comp);
        return;
    }
    enableFirstInThreeValue(first, last, comp);
    auto f = first;
    auto l = last;
    auto pivot = *first;
    --last;
    int leftEqualCnt = 0;
    int rightEqualCnt = 0;
    while(first != last)
    {
        while(first != last && (!comp(*last, pivot) || (comp(*last, pivot) && comp(pivot, *last))))
        {
            if(comp(*last, pivot) && comp(pivot, *last))
                tinystl::swap(*last, *(l - rightEqualCnt++ - 1));
            --last;
        }
        if(first != last)
            tinystl::swap(*first++, *last);
        while(first != last && comp(*first, pivot)) 
        {
            if(comp(*first, pivot) && comp(pivot, *first))
                tinystl::swap(*first, *(f + leftEqualCnt++));
            ++first;
        }
        if(first != last)
            tinystl::swap(*first, *last--);
    }
    *first = pivot;
    tinystl::sort(f + leftEqualCnt, first);
    tinystl::sort(first + 1, l - rightEqualCnt);
}

template <class ForwardIt, class Compare = std::less_equal<typename iterator_traits<ForwardIt>::value_type>>
bool is_sorted(ForwardIt first, ForwardIt last, Compare comp = Compare())
{
    while(first != last)
    {
        ForwardIt next = first;
        ++next;
        if(next != last && !comp(*first, *next))
            return false;
        first = next;
    }
    return true;
}



/* min/max */

template <class T>
const T& max(const T& lhs, const T& rhs)
{
    return lhs > rhs ? lhs : rhs;
}

template <class T>
const T& min(const T& lhs, const T& rhs)
{
    return lhs < rhs ? lhs : rhs;
}

/* Modify */
template <class ForwardIt, class T>
void fill(ForwardIt first , ForwardIt last ,const T& value)
{
    while(first != last)
        (*first++) = value;
}

template <class OutputIt, class Size, class T>
OutputIt fill_n(OutputIt first, Size count,  const T& value)
{
    while(count--)
        (*first++) = value;
    return first;
}

template <class ForwardIt, class Generator>
void generate(ForwardIt first, ForwardIt last, Generator g)
{
    tinystl::for_each(first, last, [&g](auto& value) {
                        value = g();
                      });
}

template <class OutputIt, class Size, class Generator>
void generate_n(OutputIt first, Size count, Generator g)
{
    while(count--)
    {
        *(first++) = g();
    }
}
template <class InputIt, class OutputIt, class UnaryPredicate>
OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred)
{
    while(first != last)
    {
        if(pred(*first))
            *(d_first++) = *first;
        ++first;
    }
    return d_first;
}
template <class InputIt, class OutputIt>
OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
{
    return tinystl::copy_if(first, last, d_first, [](const auto&) { return true; });
}


template <class BidirIt1, class BidirIt2>
BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
{
    --last;
    while(true)
    {
        *d_last = *last;
        if(last == first)
            return d_last;
        --d_last;
        --last;
    }
}

template <class BidirIt>
void reverse(BidirIt first, BidirIt last)
{
    if(first != last)
    {
        auto count = tinystl::distance(first, last) / 2;
        --last;
        while(count--)
        {
            tinystl::swap(*first, *last);
            ++first;
            --last;
        }
    }
}
/* Heap */

template <class RandomIterator, 
          class Compare = std::less<typename iterator_traits<RandomIterator>::value_type>>
void make_heap(RandomIterator first,
               RandomIterator last,
               Compare comp = Compare())
{
    if(first == last)   return;
    auto it = last - 1;
    while(true)
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
        if(it == first)
            break;
        --it;
    }
}

template <class RandomIterator,
          class Compare = std::less<typename iterator_traits<RandomIterator>::value_type>>
void push_heap(RandomIterator first, RandomIterator last, Compare comp = Compare())
{
    auto value = *(last - 1);
    RandomIterator currentIt = last - 1;
    RandomIterator parentIt = first + (currentIt - first + 1) / 2 - 1;
    while(currentIt != first)
    {
        if(comp(value, *parentIt))
            break;
        *currentIt = *parentIt;
        currentIt = parentIt;
        parentIt = first + (currentIt - first + 1) / 2 - 1;
    }
    *currentIt = value;
}

template <class RandomIterator,
          class Compare = std::less<typename iterator_traits<RandomIterator>::value_type>>
void pop_heap(RandomIterator first, RandomIterator last, Compare comp = Compare())
{
    tinystl::swap(*first, *(last - 1));
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

template <class RandomIt,
          class Compare = std::greater<typename iterator_traits<RandomIt>::value_type>>
void sort_heap(RandomIt first, RandomIt last, Compare comp = Compare())
{
    tinystl::make_heap(first, last, comp);
    auto count = tinystl::distance(first, last);
    RandomIt lastTmp = last;
    while(count--)
    {
        tinystl::pop_heap(first, last, comp);
        --last;
    }
    last = lastTmp;
    tinystl::reverse(first, last);
}

template <class RandomIt,
          class Compare = std::less<typename iterator_traits<RandomIt>::value_type>>
bool is_heap(RandomIt first, RandomIt last, Compare comp = Compare())
{
    RandomIt cur = first;
    while(cur != last)
    {
        RandomIt child = cur + (cur - first) + 1;
        if(child != last && comp(*child, *cur))
            return false;
        if((child + 1) != last && comp(*(child + 1), *cur))
            return false;
        ++cur;
    }
    return true;
}



}

