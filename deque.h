#pragma once

#include "iterator.h"
#include "alloc.h"
#include "construct.h"

#include <memory>
#include <iostream>

namespace tinystl
{

template <class T, size_t BufSize>
class DequeIterator : public iterator<random_access_iterator_tag, T>
{
public:
    typedef DequeIterator<T, BufSize> Self;

    typedef random_access_iterator_tag iterator_category;
    typedef T                           value_type;
    typedef T&                          reference;
    typedef T*                          pointer;
    typedef std::ptrdiff_t              difference_type;
    typedef std::size_t                 size_type;
    typedef T**                         map_pointer;

    /* [first : last)保存数据 */
    pointer first_;
    pointer last_;
    /* 当前位置 */
    pointer cur_;
    /* 当前所属的map指针 */
    map_pointer map_;


    DequeIterator() 
        : first_(nullptr), 
          last_(nullptr), 
          cur_(nullptr), 
          map_(nullptr) 
    {}

    reference operator*() { return *cur_; }
    pointer operator->() { return &(operator*()); }

    bool operator==(const Self& it) const
    {
        return first_ == it.first_ && last_ == it.last_ && cur_ == it.cur_ && map_ == it.map_;
    }
    bool operator!=(const Self& it) const
    {
        return !(*this == it);
    }

    Self& operator++()
    {
        if(++cur_ == last_)
        {
            setMapNode(map_ + 1);
            cur_ = first_;
        }
        return *this;
    }
    Self operator++(int)
    {
        Self tmp = *this;
        ++(*this);
        return tmp;
    }
    Self& operator--()
    {
        if(cur_ == first_)
        {
            setMapNode(map_ - 1);
            cur_ = last_ - 1;
        }
        else
        {
            --cur_;
        }
        return *this;
    }
    Self operator--(int)
    {
        Self tmp = *this;
        --(*this);
        return tmp;
    }
    Self& operator+=(difference_type n)
    {
        if(n == 0) return *this;
        if(cur_ + n >= last_)
        {
            n += (cur_ - first_);
            size_type mapCnt = n / bufferSize(); 
            size_type pointCnt = n % bufferSize();
            setMapNode(map_ + mapCnt);
            cur_ = first_ + pointCnt;
        }
        else
        {
            cur_ += n;
        }
        return *this;
    }
    Self& operator-=(difference_type n)
    {
        return operator+=(-n);
    }
    Self operator+(difference_type n) const
    {
        Self tmp = *this;
        return tmp += n;
    }
    Self operator-(difference_type n) const
    {
        return operator+(-n);
    }

    difference_type operator-(const Self& it) const
    {
        return static_cast<difference_type>(bufferSize()) * 
            (map_ - it.map_ - 1) + (cur_ - first_) + (it.last_ - it.cur_);
    }
    reference operator[](difference_type pos)
    {
        return *(*this + pos);
    }
    void setMapNode(map_pointer mapNode)
    {
        map_ = mapNode;
        first_ = (*map_);
        last_ = first_ + bufferSize();
    }

    size_type bufferSize() const 
    {
        return BufSize == 0 ? 10 : BufSize;
    }
};


template <class T, class Alloc = alloc, size_t BufSize = 0>
class deque
{
public:
    typedef T                           value_type;
    typedef T*                          pointer;
    typedef const T*                    const_pointer;
    typedef T&                          reference;
    typedef const T&                    const_reference;
    typedef std::size_t                 size_type;
    typedef std::ptrdiff_t              difference_type;
    typedef DequeIterator<T, BufSize>   iterator;
    typedef const iterator              const_iterator;
    typedef tinystl::reverse_iterator<iterator> reverse_iterator;
    typedef const reverse_iterator      const_reverse_iterator;

    typedef T**                         map_pointer;
    typedef SimpleAlloc<value_type, Alloc> dataAllocator;
    typedef SimpleAlloc<pointer, Alloc>    mapAllocator;
private:
    /* [start_, finish_]表示数据范围迭代器 */
    /* [start_.cur_, finish_.cur)表示数据范围 */
    iterator start_;
    iterator finish_;
    map_pointer map_;
    /* map个数 */
    size_type mapSize_;

public:
    deque();
    explicit deque(size_type count);
    deque(size_type count, const value_type& value);
    template <class InputIterator>
        deque(InputIterator first, InputIterator last);
    deque(const deque& other);
    deque(deque&& other);
    deque(std::initializer_list<T> init);

    ~deque();

    deque& operator=(const deque& other);
    deque& operator=(deque&& other) noexcept;
    deque& operator=(std::initializer_list<T> ilist);

    void assign(size_type count, const value_type& value)
    { std::swap(deque(count, value), *this); }
    template <class InputIterator>
        void assign(InputIterator first, InputIterator last)
        { ; std::swap(deque(first, last), *this); }
    void assign(std::initializer_list<T> ilist)
    { std::swap(deque(ilist), *this); }

    reference at(size_type pos); 
    const_reference at(size_type pos) const; 

    reference operator[](size_type pos) { return start_[static_cast<difference_type>(pos)]; }
    const_reference operator[](size_type pos) const { return start_[static_cast<difference_type>(pos)]; }

    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *(--end()); }
    const_reference back() const { return *(--end()); }

    iterator begin() noexcept { return start_; }
    const_iterator begin() const noexcept { return start_; }
    const_iterator cbegin() const noexcept { return start_; }
    reverse_iterator rbegin() noexcept { return reverse_iterator(finish_); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(finish_); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(finish_); }

    iterator end() noexcept { return finish_; }
    const_iterator end() const noexcept { return finish_; }
    const_iterator cend() const noexcept { return finish_; }
    reverse_iterator rend() noexcept { return reverse_iterator(start_); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(start_); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(start_); }

    bool empty() const noexcept { return begin() == end(); }
    size_type size() const noexcept { return finish_ - start_; }
    size_type max_size() const noexcept { return static_cast<size_type>(-1); }

    void clear() noexcept; 

    /* iterator insert(iterator pos, const value_type& value); */
    /* iterator insert(iterator pos, size_type count, const value_type& value); */
    /* template <class InputIterator> */
    /*     iterator insert(iterator pos, InputIterator first, InputIterator last); */
    /* iterator insert(iterator pos, std::initializer_list<T> ilist); */
    
    /* iterator erase(iterator first, iterator last); */

    void push_back(const value_type& value);
    void push_front(const value_type& value);
    void pop_back();
    void pop_front();

    void resize(size_type count) { resize(count, value_type()); }
    void resize(size_type count, const value_type& value);

    void swap(deque& other) noexcept; 
private:

    void createMap(size_type count);
    void reverse_map_at_front(size_type addCount);
    void reverse_map_at_back(size_type addCount);
    void reallocate_map(size_type addCount);

    void ctorAux(size_type count, const value_type& value, std::true_type);
    template <class InputIterator>
        void ctorAux(InputIterator first, InputIterator last, std::false_type);

    /* iterator insertAux(iterator pos, size_type count, const value_type& value, std::true_type); */
    /* template <class InputIterator> */
    /*     iterator insertAux(iterator pos, InputIterator first, InputIterator last, std::false_type); */

private:
    size_type bufferSize() const 
    {
        return BufSize == 0 ? 10 : BufSize;
    }
    size_type initializeSize() const
    {
        return static_cast<size_type>(8);
    }
};


template <class T, class Alloc, size_t BufSize>
deque<T, Alloc, BufSize>::deque()
    : deque(0)
{

}


template <class T, class Alloc, size_t BufSize>
deque<T, Alloc, BufSize>::deque(size_type count)
    : deque(count, value_type())
{

}

template <class T, class Alloc, size_t BufSize>
deque<T, Alloc, BufSize>::deque(size_type count, const value_type& value)
{
    ctorAux(count, value, std::is_integral<size_type>());
}

template <class T, class Alloc, size_t BufSize>
template <class InputIterator>
deque<T, Alloc, BufSize>::deque(InputIterator first, InputIterator last)
{
    ctorAux(first, last, std::is_integral<InputIterator>());
}


template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::createMap(size_type count)
{
    size_type mapNodeSize = count / bufferSize() + 1;
    mapSize_ = std::max(initializeSize(),  mapNodeSize + 2);
    map_ = mapAllocator::allocate(mapSize_);
    map_pointer mapStart = map_ + (mapSize_ - mapNodeSize) / 2;
    map_pointer mapEnd = mapStart + mapNodeSize - 1;
    for(map_pointer cur = mapStart; cur <= mapEnd; ++cur)
        *cur = dataAllocator::allocate(bufferSize());
    start_.setMapNode(mapStart);
    finish_.setMapNode(mapEnd);
    start_.cur_ = start_.first_;
    finish_.cur_ = finish_.first_ + count % bufferSize();
}


template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::ctorAux(size_type count, const value_type& value, std::true_type)
{
    createMap(count);
    for(map_pointer cur = start_.map_; cur != finish_.map_; ++cur)
        std::uninitialized_fill(*cur, *cur + bufferSize(), value);
    std::uninitialized_fill(finish_.first_, finish_.cur_, value);
}

template <class T, class Alloc, size_t BufSize>
template <class InputIterator>
void deque<T, Alloc, BufSize>::ctorAux(InputIterator first, InputIterator last, std::false_type)
{
    auto count = last - first; 
    createMap(count);
    for(map_pointer cur = start_.map_; cur != finish_.map_; ++cur)
    {
        std::uninitialized_copy(first, first + bufferSize(), *cur);
        first += bufferSize();
    }
    std::uninitialized_copy(first, last, finish_.first_);
}

template <class T, class Alloc, size_t BufSize>
deque<T, Alloc, BufSize>::deque(std::initializer_list<T> init)
    : deque(init.begin(), init.end())
{

}


template <class T, class Alloc, size_t BufSize>
deque<T, Alloc, BufSize>::deque(const deque& other)
    : deque(other.begin(), other.end())
{

}


template <class T, class Alloc, size_t BufSize>
deque<T, Alloc, BufSize>::deque(deque&& other)
    : start_(std::move(other.start_)),
      finish_(std::move(other.finish_)),
      map_(std::move(other.map_)),
      mapSize_(std::move(other.mapSize_))
{
    other.start_ = other.finish_ = deque::iterator();
    other.map_ = nullptr;
    other.mapSize_ = 0;
}

template <class T, class Alloc, size_t BufSize>
deque<T, Alloc, BufSize>::~deque()
{
    clear();
    for(map_pointer cur = start_.map_; cur && cur <= finish_.map_; ++cur)
        dataAllocator::deallocate(*cur);
    mapAllocator::deallocate(map_);
}


template <class T, class Alloc, size_t BufSize>
deque<T, Alloc, BufSize>&
deque<T, Alloc, BufSize>::operator=(const deque& other)
{
    if(&other != this)
    {
        std::swap(deque(other), *this);
    }
    return *this;
}


template <class T, class Alloc, size_t BufSize>
deque<T, Alloc, BufSize>&
deque<T, Alloc, BufSize>::operator=(deque&& other) noexcept
{
    if(&other != this)
    {
        std::swap(deque(std::move(other)), *this);
    }
    return *this;
}


template <class T, class Alloc, size_t BufSize>
deque<T, Alloc, BufSize>&
deque<T, Alloc, BufSize>::operator=(std::initializer_list<T> ilist)
{
    std::swap(deque(ilist), *this);
    return *this;
}


template <class T, class Alloc, size_t BufSize>
typename deque<T, Alloc, BufSize>::reference
deque<T, Alloc, BufSize>::at(size_type pos) 
{ 
    if(!(pos < size()))
        throw std::out_of_range("out of range");
    return start_[static_cast<difference_type>(pos)]; 
}

template <class T, class Alloc, size_t BufSize>
typename deque<T, Alloc, BufSize>::const_reference
deque<T, Alloc, BufSize>::at(size_type pos) const 
{ 
    if(!(pos < size()))
        throw std::out_of_range("out of range");
    return start_[static_cast<difference_type>(pos)]; 
}



template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::clear() noexcept
{
    destroy(begin(), end());
    finish_ = start_;
}

/* template <class T, class Alloc, size_t BufSize> */
/* typename deque<T, Alloc, BufSize>::iterator */
/* deque<T, Alloc, BufSize>::insert(iterator pos, const value_type& value) */
/* { */
/*     return insert(pos, 1, value); */
/* } */

/* template <class T, class Alloc, size_t BufSize> */
/* typename deque<T, Alloc, BufSize>::iterator */
/* deque<T, Alloc, BufSize>::insert(iterator pos, size_type count, const value_type& value) */
/* { */
/*     return insertAux(pos, count, value, std::is_integral<size_type>()); */
/* } */

/* template <class T, class Alloc, size_t BufSize> */
/* template <class InputIterator> */
/* typename deque<T, Alloc, BufSize>::iterator */
/* deque<T, Alloc, BufSize>::insert(iterator pos, InputIterator first, InputIterator last) */ 
/* { */
/*     return insertAux(pos, first, last, std::is_integral<InputIterator>()); */
/* } */

/* template <class T, class Alloc, size_t BufSize> */
/* typename deque<T, Alloc, BufSize>::iterator */
/* deque<T, Alloc, BufSize>::insertAux(iterator pos, size_type count, const value_type& value, std::true_type) */
/* { */
/*     const size_type disFromStart = pos - start_; */
/*     const size_type disToFinish = finish_ - pos; */
/*     if(disFromStart < disToFinish) */
/*     { */
/*         for(size_type i = 0; i < count; ++i) */
/*             push_front(front()); */
/*         std::copy(begin() + count, pos, begin()); */
/*         std::fill_n(pos - count, count, value); */
/*         return pos - count; */
/*     } */
/*     else */
/*     { */
/*         for(size_type i = 0; i < count; ++i) */
/*             push_back(back()); */
/*         std::copy_backward(pos, end() - count, end() - 1); */
/*         std::fill_n(pos, count, value); */
/*         return pos; */
/*     } */
/* } */

/* template <class T, class Alloc, size_t BufSize> */
/* template <class InputIterator> */
/* typename deque<T, Alloc, BufSize>::iterator */
/* deque<T, Alloc, BufSize>::insertAux(iterator pos, InputIterator first, InputIterator last, std::false_type) */ 
/* { */   
/*     const size_type disFromStart = pos - start_; */
/*     const size_type disToFinish = finish_ - pos; */
/*     const size_type count = std::distance(first, last); */
/*     if(disFromStart < disToFinish) */
/*     { */
/*         for(size_type i = 0; i < count; ++i) */
/*             push_front(front()); */
/*         std::copy(begin() + count, pos, begin()); */
/*         std::copy(first, last, pos - count); */
/*         return pos - count; */
/*     } */
/*     else */
/*     { */
/*         for(size_type i = 0; i < count; ++i) */
/*             push_back(back()); */
/*         std::copy_backward(pos, end() - count, end() - 1); */
/*         std::copy(first, last, pos); */
/*         return pos; */
/*     } */
/* } */

/* template <class T, class Alloc, size_t BufSize> */
/* typename deque<T, Alloc, BufSize>::iterator */
/* deque<T, Alloc, BufSize>::insert(iterator pos, std::initializer_list<T> ilist) */
/* { */
/*     return insert(pos, ilist.begin(), ilist.end()); */
/* } */



template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::reverse_map_at_front(size_type addCount)
{
    if(start_.map_ == map_ && start_.cur_ - addCount < start_.first_)
    {
        reallocate_map(addCount);
    }
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::reverse_map_at_back(size_type addCount)
{
    if(finish_.map_ == map_ + mapSize_ - 1 && finish_.cur_ + addCount >= finish_.last_)
    {
        reallocate_map(addCount);
    }
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::reallocate_map(size_type addCount)
{
    size_type oldMapSize = finish_.map_ - start_.map_ + 1;
    size_type newMapSize = mapSize_ + std::max(mapSize_, addCount) + 2;
    map_pointer newMap = mapAllocator::allocate(newMapSize);
    map_pointer newStart = newMap + (newMapSize - (oldMapSize + addCount)) / 2;
    map_pointer newFinish = newStart + oldMapSize + addCount - 1;
    /* 不需要copy数据，只需要copy指针即可 */
    std::uninitialized_copy(start_.map_, finish_.map_ + 1, newStart);
    /* 只需要释放申请的map指针，实际数据保留 */
    mapAllocator::deallocate(map_);
    map_ = newMap;
    mapSize_ = newMapSize;
    start_.setMapNode(newStart);
    finish_.setMapNode(newStart + oldMapSize - 1);
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::push_back(const value_type& value)
{
    /* finish_.cur_指向的是下一个可以存放数据的位置 */
    if(finish_.cur_ == finish_.last_  - 1)
    {
        reverse_map_at_back(1);
        *(finish_.map_ + 1) = dataAllocator::allocate(bufferSize());
    }
    construct(finish_.cur_, value);
    ++finish_;
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::push_front(const value_type& value)
{
    if(start_.cur_ == start_.first_)
    {
        reverse_map_at_front(1);
        *(start_.map_ - 1) = dataAllocator::allocate(bufferSize());
    }
    
    --start_;
    construct(start_.cur_, value);
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::pop_front()
{
    if(empty()) return;
    destroy(start_.cur_);
    ++start_;
    if(start_.cur_ == start_.first_)
    {
        dataAllocator::deallocate(*(start_.map_ - 1));
    }
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::pop_back()
{
    if(empty()) return;
    destroy(finish_.cur_);
    --finish_;
    if(finish_.cur_ == finish_.last_ - 1)
    {
        dataAllocator::deallocate(*(finish_.map_ + 1));
    }
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::resize(size_type count, const value_type& value)
{
    size_type curSize = size();
    if(count > curSize)
    {
        count -= curSize;
        while(count--)
            push_back(value);
    }
    else
    {
        curSize -= count;
        while(curSize--)
            pop_back();
    }
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::swap(deque& other) noexcept
{
    std::swap(start_, other.start_);
    std::swap(finish_, other.finish_);
    std::swap(map_, other.map_);
    std::swap(mapSize_, other.mapSize_);
}


template <class T, class Alloc, size_t BufSize>
bool operator==(const deque<T, Alloc, BufSize>& lhs,
                const deque<T, Alloc, BufSize>& rhs)
{
    if(lhs.size() != rhs.size())
        return false;
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    while(lit != lhs.end())
    {
        if(*lit++ != *rit++)
            return false;
    }
    return true;
}
template <class T, class Alloc, size_t BufSize>
bool operator!=(const deque<T, Alloc, BufSize>& lhs,
                const deque<T, Alloc, BufSize>& rhs)
{
   return !(lhs == rhs); 
}

}
