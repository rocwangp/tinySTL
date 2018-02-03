#pragma once

#include <type_traits>
#include <algorithm>

#include <cstdint>
#include "alloc.h"
#include "memory.h"
#include "iterator.h"
#include "algorithm.h"

namespace tinystl{

/* vector定义，T是数据类型，Alloc是空间配置器，默认使用第一级配置器(对malloc的封装) */
template <class T, class Alloc = alloc>
class vector {
public:
    /* 内嵌类型定义 */
    typedef T                               value_type;
    typedef T*                              pointer;
    typedef const T*                        const_pointer;
    typedef T&                              reference;
    typedef const T&                        const_reference;
    typedef T*                              iterator;
    typedef tinystl::reverse_iterator<iterator>      reverse_iterator;
    typedef const reverse_iterator          const_reverse_iterator;
    typedef const T*                        const_iterator;
    typedef std::ptrdiff_t                  difference_type;
    typedef std::size_t                     size_type;

private:
    /* 空间配置器的又一层封装，可以将申请的地址类型从void*转为value_type* */
    typedef SimpleAlloc<value_type, Alloc> dataAllocator;
    /* start_ : 内存起始位置
     * finish_: 使用的内存终止位置
     * end_   : 申请的内存终止位置 */
    iterator start_;
    iterator finish_;
    iterator end_;

public:
    /* 构造，析构，赋值，初始化 */
    vector();
    explicit vector(size_type count);
    vector(size_type count, const value_type& value);
    template <class InputIterator>
        vector(InputIterator first, InputIterator last);
    vector(const vector& other);
    vector(vector&& other) noexcept;
    vector(std::initializer_list<T> init);

    ~vector();

    vector& operator=(const vector& other);
    vector& operator=(vector&& other) noexcept;
    vector& operator=(std::initializer_list<T> ilist);

    void assign(size_type count, const value_type& value);
    template <class InputIterator>
        void assign(InputIterator first, InputIterator last);
    void assign(std::initializer_list<T> ilist);


    /* Element access */
    reference at(size_type pos);
    const_reference at(size_type pos) const;

    reference operator[](size_type pos) { return *(begin() + pos); }
    const_reference operator[](size_type pos) const { return *(begin() + pos); }

    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *(end() - 1); }
    const_reference back() const { return const_reference(*(end() - 1)); }


    pointer data() noexcept { return begin(); }
    const_pointer data() const noexcept { return static_cast<const_pointer>(begin()); }

    /* Iterators */
    iterator begin() { return start_; }
    iterator end()  { return finish_; }
    const_iterator begin() const noexcept { return static_cast<const_iterator>(start_); }
    const_iterator end() const noexcept{ return static_cast<const_iterator>(finish_); }
    const_iterator cbegin() const noexcept { return static_cast<const_iterator>(start_); }
    const_iterator cend() const noexcept { return static_cast<const_iterator>(finish_); }
    reverse_iterator rbegin()  noexcept { return reverse_iterator(finish_); }
    reverse_iterator rend()  noexcept { return reverse_iterator(start_); }
    const_reverse_iterator rbegin() const noexcept 
    { return static_cast<const_reverse_iterator>(reverse_iterator(finish_)); }
    const_reverse_iterator rend() const noexcept 
    { return static_cast<const_reverse_iterator>(reverse_iterator(start_)); }
    const_reverse_iterator crbegin() const noexcept 
    { return static_cast<const_reverse_iterator>(reverse_iterator(finish_)); }
    const_reverse_iterator crend() const noexcept 
    { return static_cast<const_reverse_iterator>(reverse_iterator(finish_)); }

    /* Capacity */
    bool empty() const noexcept { return begin() == end(); }
    size_type size() const noexcept { return static_cast<size_type>(tinystl::distance(begin(), end())); }
    size_type max_size() const noexcept { return size(); }


    void reserve(size_type new_cap);
    size_type capacity() const noexcept{ return static_cast<size_type>(tinystl::distance(start_, end_)); }

    void shrink_to_fit();

    /* Modifiers */
    void clear() noexcept { erase(begin(), end()); }

    iterator insert(iterator pos, const value_type& value);
    iterator insert(iterator pos, size_type count, const value_type& value);
    template <class InputIterator>
        iterator insert(iterator pos, InputIterator first, InputIterator last);
    iterator insert(iterator pos, std::initializer_list<T> ilist); 

    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);

    void push_back(const value_type& value);
    void pop_back();


    void resize(size_type count);
    void resize(size_type count, const value_type& value); 

    void swap(vector& v) noexcept;

private:
    void ctorAux(size_type count, const value_type& value, std::true_type);
    template <class InputIterator>
        void ctorAux(InputIterator first, InputIterator last, std::false_type);

    iterator insertAux(iterator pos, size_type count, const value_type& value, std::true_type);
    template <class InputIterator>
        iterator insertAux(iterator pos, InputIterator first, InputIterator last, std::false_type);
};



/* **************************构造，析构，初始化，赋值相关***************************** */
template <class T, class Alloc>
vector<T, Alloc>::vector()
    : start_(nullptr),
      finish_(nullptr),
      end_(nullptr)
{  }

template <class T, class Alloc>
vector<T, Alloc>::vector(size_type count)
    : vector(count, value_type())
{

}

template <class T, class Alloc>
void vector<T, Alloc>::ctorAux(size_type count, const value_type& value, std::true_type)
{
    start_ = dataAllocator::allocate(count);
    finish_ = start_ + count;
    end_ = finish_;
    tinystl::uninitialized_fill_n(start_, count, value);
}

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::ctorAux(InputIterator first, InputIterator last, std::false_type)
{
    start_ = dataAllocator::allocate(last - first);
    finish_ = start_ + (last - first);
    end_ = finish_;
    tinystl::uninitialized_copy(first, last, start_);
}

template <class T, class Alloc>
vector<T, Alloc>::vector(size_type count, const value_type& value)
{
    ctorAux(count, value, std::is_integral<size_type>());
}

template <class T, class Alloc>
template <class InputIterator>
vector<T, Alloc>::vector(InputIterator front, InputIterator back)
{
    ctorAux(front, back, std::is_integral<InputIterator>());
}


/* 拷贝构造函数 */
template <class T, class Alloc>
vector<T, Alloc>::vector(const vector& other)
    : start_(dataAllocator::allocate(other.size())),
      finish_(start_ + other.size()),
      end_(start_ + other.size())
{
    tinystl::uninitialized_copy(other.begin(), other.end(), start_);
}

template <class T, class Alloc>
vector<T, Alloc>::vector(vector&& other) noexcept
    : start_(std::move(other.start_)),
      finish_(std::move(other.finish_)),
      end_(std::move(other.end_))
{
    other.start_ = other.finish_ = other.end_ = nullptr;
}

template <class T, class Alloc>
vector<T, Alloc>::vector(std::initializer_list<T> init)
    : vector(init.begin(), init.end())
{

}


/* 析构函数，释放内存 */
template <class T, class Alloc>
vector<T, Alloc>::~vector()
{
    clear();
    dataAllocator::deallocate(start_);
}


/* 拷贝赋值运算符 */
template <class T, class Alloc>
vector<T, Alloc>& 
vector<T, Alloc>::operator=(const vector& other)
{
    if(&other != this)
    {
        tinystl::swap(vector(other), *this);
    }
    return *this;
}




/* 移动赋值运算符 */
template <class T, class Alloc>
vector<T, Alloc>&
vector<T, Alloc>::operator=(vector&& other) noexcept
{
    if(&other != this)
    {
        vector tmp(std::move(other));
        tinystl::swap(tmp, *this);
    }
    return *this;
}



/* 移动赋值运算符 */
template <class T, class Alloc>
vector<T, Alloc>&
vector<T, Alloc>::operator=(std::initializer_list<T> ilist) 
{
    vector tmp(ilist);
    tinystl::swap(tmp, *this);
    return *this;
}


template <class T, class Alloc>
void vector<T, Alloc>::assign(size_type count, const value_type& value)
{
    vector tmp(count, value);
    tinystl::swap(tmp, *this);
}

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::assign(InputIterator first, InputIterator last)
{
    vector tmp(first, last);
    tinystl::swap(tmp, *this);
}

template <class T, class Alloc>
void vector<T, Alloc>::assign(std::initializer_list<T> ilist)
{
    vector tmp(ilist.begin(), ilist.end());
    tinystl::swap(tmp, *this);
}


/* **********************************成员访问相关************************************ */
template <class T, class Alloc>
typename vector<T, Alloc>::reference
vector<T, Alloc>::at(size_type pos)
{
    if(pos >= size())
        throw std::out_of_range("out of range");
    return *(begin() + pos);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reference
vector<T, Alloc>::at(size_type pos) const
{
    if(pos >= size())
        throw std::out_of_range("out of range");
    return *(begin() + pos);
}

/* *********************************容量大小相关************************************** */
template <class T, class Alloc>
void vector<T, Alloc>::reserve(size_type new_cap)
{
    if(new_cap > capacity())
    {
        const size_type oldSize = size();
        iterator start = dataAllocator::allocate(new_cap);
        tinystl::uninitialized_copy(begin(), end(), start);
        destroy(begin(), end());
        dataAllocator::deallocate(begin());
        start_ = start;
        finish_ = start_ + oldSize;
        end_ = start_ + new_cap;
    }
}

template <class T, class Alloc>
void vector<T, Alloc>::shrink_to_fit()
{
    if(size() == capacity())
        return;
    const size_type n = size();
    iterator start = dataAllocator::allocate(n);
    tinystl::uninitialized_copy(begin(), end(), start);
    destroy(begin(), end());
    dataAllocator::deallocate(begin());
    start_ = start;
    finish_ = end_ = start_ + n;
}


/* **************************************修改相关****************************************** */
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(iterator pos, const value_type& value) 
{
    insert(pos, 1, value);
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(iterator pos, size_type count, const value_type& value) 
{
    return insertAux(pos, count, value, std::is_integral<size_type>());
}

template <class T, class Alloc>
template <class InputIterator>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(iterator pos, InputIterator first, InputIterator last)
{
    return insertAux(pos, first, last, std::is_integral<InputIterator>());
}



template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insertAux(iterator pos, size_type count, const value_type& value, std::true_type)
{
    const size_type prevSize = tinystl::distance(begin(), pos); 
    if(count != 0) 
    {
        if(static_cast<size_type>(end_ - finish_) >= count) 
        {
            if(count >= end() - pos)
            {
                iterator last = tinystl::uninitialized_fill_n(end(), count - (end() - pos), value);
                tinystl::uninitialized_copy(pos, end(), last);
                tinystl::fill(pos, end(), value);
            }
            else
            {
                tinystl::uninitialized_copy(end() - count, end(), end());
                tinystl::copy_backward(pos, end() - count, end());
                tinystl::fill_n(pos, count, value);
            }
            finish_ += count;
        }
        else 
        {
            const size_type oldSize = size();
            const size_type newSize = oldSize + tinystl::max(oldSize, count);
            iterator start = dataAllocator::allocate(newSize);
            iterator it = tinystl::uninitialized_copy(begin(), pos, start);
            it = tinystl::uninitialized_fill_n(it, count, value);
            tinystl::uninitialized_copy(pos, end(), it);
            destroy(begin(), end());
            dataAllocator::deallocate(start_);
            start_ = start;
            finish_ = start_ + oldSize + count;
            end_ = start_ + newSize;
        }
    }
    return begin() + prevSize;
}

template <class T, class Alloc>
template <class InputIterator>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insertAux(iterator pos, InputIterator first, InputIterator last, std::false_type)
{
    const size_type prevSize = tinystl::distance(begin(), pos); 
    const size_type count = last - first; 
    if(static_cast<size_type>(end_ - finish_) >= count)
    {
        if(count >= end() - pos)
        {
            tinystl::uninitialized_copy(pos, end(), pos + count);
            tinystl::copy(first, last, pos);
        }
        else
        {
            tinystl::uninitialized_copy(end() - count, end(), end());
            tinystl::copy_backward(pos, pos + count, end());
            tinystl::copy(first, last, pos);
        }
        finish_ += count;
    }
    else
    {
        const size_type oldSize = size();
        const size_type newSize = oldSize + tinystl::max(oldSize, count);
        iterator start = dataAllocator::allocate(newSize);
        iterator it = tinystl::uninitialized_copy(begin(), pos, start);
        it = tinystl::uninitialized_copy(first, last, it);
        tinystl::uninitialized_copy(pos, end(), it);
        start_ = start;
        finish_ = start_ + oldSize + count;
        end_ = start_ + newSize;
    }
    return begin() + prevSize;
}


template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(iterator pos) 
{
    return erase(pos, pos + 1);
}

/* 删除[front, back)范围的元素 */
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(iterator first, iterator last) 
{
    const auto eraseSize = last - first;
    const auto lastSize = end() - last;
    const auto prevSize = first - begin();
    const auto uninitCopySize = tinystl::min(eraseSize, lastSize);
    destroy(first, last);
    iterator it = tinystl::uninitialized_copy(last, last + uninitCopySize, first);
    tinystl::copy(last + uninitCopySize, end(), it);
    destroy(end() - uninitCopySize, end());
    finish_ = end() - eraseSize;
    return begin() + prevSize;
}

/* 在末尾追加数据，如果申请的内存空间用完，则重新分配内存 */
template <class T, class Alloc>
void vector<T, Alloc>::push_back(const value_type& value)
{
    /* 在末尾插入，会判断是否需要重新申请内存 */
    insert(end(), value);
}

template <class T, class Alloc>
void vector<T, Alloc>::pop_back() 
{
    destroy(--finish_);
}

template <class T, class Alloc>
void vector<T, Alloc>::resize(size_type count) 
{
    resize(count, T());
}

template <class T, class Alloc>
void vector<T, Alloc>::resize(size_type count, const T& value) 
{
    if(count >= size())
        insert(end(), count - size(), value);
    else
        erase(begin() + count, end());
}

template <class T, class Alloc>
void vector<T, Alloc>::swap(vector& other) noexcept
{
    tinystl::swap(start_, other.start_);
    tinystl::swap(finish_, other.finish_);
    tinystl::swap(end_, other.end_);
}


/* ****************************************8运算符相关*************************************** */
template <class T, class Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
{
    if(lhs.size() != rhs.size())
        return false;
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    auto n = lhs.size();
    while(n--)
    {
        if(*lit++ != *rit++)
            return false;
    }
    return true;
}


template <class T, class Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <class T, class Alloc>
void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs)
{
    lhs.swap(rhs);
}

}

