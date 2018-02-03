#pragma once

#include "alloc.h"
#include "construct.h"
#include "vector.h"
#include "algorithm.h"

#include <memory>
#include <iterator>
#include <algorithm>
#include <unordered_map>

namespace tinystl
{

template <class T, class Traits = std::char_traits<T>, class Alloc = alloc>
class basic_string
{
public:
    typedef Traits                              traits_type;
    typedef T                                   value_type;
    typedef T*                                  pointer;
    typedef const T*                       const_pointer;
    typedef T&                                  reference;
    typedef const T&                            const_reference;
    typedef T*                                  iterator;
    typedef const iterator                      const_iterator;
    typedef tinystl::reverse_iterator<iterator> reverse_iterator;
    typedef const reverse_iterator              const_reverse_iterator;
    typedef std::ptrdiff_t                      difference_type;
    typedef std::size_t                         size_type;

    typedef SimpleAlloc<T, Alloc>               dataAllocator;
    static const size_type npos;
private:
    iterator start_;
    iterator finish_;
    iterator end_;

public:
    basic_string() : start_(nullptr), finish_(nullptr), end_(nullptr) {} 
    basic_string(value_type value) : basic_string(1, value) {}
    basic_string(size_type count, value_type value);
    basic_string(const basic_string& other, size_type pos, size_type count = npos);
    basic_string(const_pointer s, size_type count);
    basic_string(const_pointer s);
    template <class InputIterator>
        basic_string(InputIterator first, InputIterator last);
    basic_string(const basic_string& other);
    basic_string(basic_string&& other) noexcept;
    basic_string(std::initializer_list<T> init);

    ~basic_string();

    basic_string& operator=(const basic_string& other)
    { basic_string tmp(other); this->swap(tmp); return *this; }
    basic_string& operator=(basic_string&& other) noexcept
    { basic_string tmp(std::move(other)); this->swap(tmp); return *this; }
    basic_string& operator=(const pointer s)
    { basic_string tmp(s); this->swap(tmp); return *this; }
    basic_string& operator=(std::initializer_list<T> ilist)
    { basic_string tmp(ilist); this->swap(tmp); return *this; }

    basic_string& assign(size_type count, value_type ch)
    { basic_string tmp(count, ch); this->swap(tmp); return *this; }
    basic_string& assign(const basic_string& str)
    { basic_string tmp(str); this->swap(tmp); return *this; }
    basic_string& assign(const basic_string& str, size_type pos, size_type count = npos)
    { basic_string tmp(str, pos, count); this->swap(tmp); return *this; }
    basic_string& assign(basic_string&& str)
    { basic_string tmp(std::move(str)); this->swap(tmp); return *this; }

    reference at(size_type pos);
    const_reference at(size_type pos) const;

    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const; 

    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }

    reference back() { iterator tmp = end(); return *(--tmp); }
    const_reference back() const { return *(--end()); }

    const_pointer data() const { return c_str(); }
    const_pointer c_str() const; 

    iterator begin() { return start_; }
    const_iterator begin() const { return start_; }
    const_iterator cbegin() const { return start_; }
    reverse_iterator rbegin() { return reverse_iterator(finish_); }
    const_reverse_iterator rbegin() const { return reverse_iterator(finish_); }
    const_reverse_iterator crbegin() const { return reverse_iterator(finish_); }

    iterator end() { return finish_; }
    const_iterator end() const { return finish_; }
    const_iterator cend() const { return finish_; }
    reverse_iterator rend() { return reverse_iterator(start_); }
    const_reverse_iterator rend() const { return reverse_iterator(start_); }
    const_reverse_iterator crend() const { return reverse_iterator(start_); }

    bool empty() const { return begin() == end(); }
    size_type size() const { return tinystl::distance(begin(), end()); } 
    size_type length() const { return size(); }
    size_type max_size() const { return static_cast<size_type>(-1); }

    void reserve(size_type newCap = 0);
    size_type capacity() const { return tinystl::distance(start_, end_); }

    void shrink_to_fit();

    void clear() { erase(begin(), end()); }

    basic_string& insert(size_type index, size_type count, value_type ch);
    basic_string& insert(size_type index, const_pointer s)
    { return insert(index, s, traits_type::length(s)); }
    basic_string& insert(size_type index, const_pointer s, size_type count);
    basic_string& insert(size_type index, const basic_string& str)
    { return insert(index, str, 0, str.size()); }
    basic_string& insert(size_type index, const basic_string& str,
                         size_type index_str, size_type count = npos);

    iterator insert(const_iterator pos, value_type ch);
    iterator insert(const_iterator pos, size_type count, value_type ch);
    template <class InputIterator>
        iterator insert(const_iterator pos, InputIterator first, InputIterator last);
    iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    basic_string& erase(size_type index = 0, size_type count = npos);
    iterator erase(iterator position) { erase(position, position + 1); }
    iterator erase(const_iterator first, const_iterator last);

    void push_back(value_type ch) { insert(end(), ch); }
    void pop_back() { iterator it = end(); erase(--it); }

    basic_string& append(size_type count, value_type ch);
    basic_string& append(const basic_string& str);
    basic_string& append(const basic_string& str, size_type pos, size_type count = npos);
    basic_string& append(const_pointer s, size_type count);
    basic_string& append(const_pointer s);
    template <class InputIterator>
        basic_string& append(InputIterator first, InputIterator last);
    basic_string& append(std::initializer_list<T> ilist);

    basic_string& operator+=(const basic_string& str)
    { return append(str); }
    basic_string& operator+=(value_type ch)
    { return append(1, ch); }
    basic_string& operator+=(const_pointer s)
    { return append(s); }
    basic_string& operator+=(std::initializer_list<T> ilist)
    { return append(ilist); }

    int compare(const basic_string& str) const
    { return compare(0, size(), str); }
    int compare(size_type pos1, size_type count1, const basic_string& str) const
    { return compare(pos1, count1, str, 0, str.size()); }
    int compare(const_pointer s) const
    { return compare(0, size(), s); }
    int compare(size_type pos1, size_type count1, const_pointer s) const
    { return compare(pos1, count1, s, traits_type::length(s)); }

    int compare(size_type pos1, size_type count1, 
                const basic_string& str,
                size_type pos2, size_type count2 = npos) const;
    int compare(size_type pos1, size_type count1, 
                const_pointer s, size_type count2) const;

    basic_string& replace(size_type pos, size_type count, const basic_string& str)
    { return replace(begin() + pos, begin() + pos + count, str); }
    basic_string& replace(const_iterator first, const_iterator last, const basic_string& str)
    { return replace(first, last, str.begin(), str.end()); }
    
    basic_string& replace(size_type pos, size_type count, 
                          const_pointer cStr, size_type count2)
    { return replace(begin() + pos, begin() + pos + count, (cStr), (cStr) + count2); }
    basic_string& replace(const_iterator first, const_iterator last,
                          const_pointer cStr, size_type count2)
    { return replace(first, last, cStr, cStr + count2); }
    basic_string& replace(size_type pos, size_type count, const_pointer cStr)
    { return replace(pos, count, cStr, traits_type::length(cStr)); }
    basic_string& replace(const_iterator first, const_iterator last, const_pointer cStr)
    { return replace(first, last, (cStr), (cStr)); }
    basic_string& replace(size_type pos, size_type count, size_type count2, value_type ch)
    { return replace(begin() + pos, begin() + pos + count, count2, ch); }

    basic_string& replace(const_iterator first, const_iterator last, size_type count2, value_type ch);
    basic_string& replace(size_type pos, size_type count,
                          const basic_string& str,
                          size_type pos2, size_type count2 = npos);
    template <class InputIterator>
        basic_string& replace(const_iterator first, const_iterator last,
                              InputIterator first2, InputIterator last2);

    basic_string substr(size_type pos = 0,
                         size_type count = npos) const;
    size_type copy(pointer dest, size_type count, size_type pos = 0) const;

    void resize(size_type count) { resize(count, value_type()); }
    void resize(size_type count, value_type ch);

    void swap(basic_string& other) noexcept 
    {
        tinystl::swap(start_, other.start_);
        tinystl::swap(finish_, other.finish_);
        tinystl::swap(end_, other.end_);
    }

    size_type find(const basic_string& str, size_type pos = 0) const
    { return find(str.data(), pos); }
    size_type find(const_pointer s, size_type pos = 0) const
    { return find(s, pos, traits_type::length(s)); }
    size_type find(value_type ch, size_type pos = 0) const
    {
        const_pointer p = data() + pos;
        const_pointer findPos = traits_type::find(p, size() - pos, ch);
        if(findPos == nullptr)  return npos;
        else    return tinystl::distance(data(), findPos);
    }
    size_type find(const_pointer s, size_type pos, size_type count) const;


    /* 功能不明确 */
    size_type rfind(const basic_string& str, size_type pos = npos) const;
    size_type rfind(const_pointer s, size_type pos = npos) const;
    size_type rfind(value_type ch, size_type pos = npos) const;
    size_type rfind(const_pointer s, size_type pos, size_type count) const;


    size_type find_first_of(const basic_string& str, size_type pos = 0) const;
    size_type find_first_of(const_pointer s, size_type pos = 0) const;
    size_type find_first_of(value_type ch, size_type pos = 0) const;
    size_type find_first_of(const_pointer s, size_type pos, size_type count) const;


    size_type find_first_not_of(const basic_string& str, size_type pos = 0) const;
    size_type find_first_not_of(const_pointer s, size_type pos = 0) const;
    size_type find_first_not_of(value_type ch, size_type pos = 0) const;
    size_type find_first_not_of(const_pointer s, size_type pos, size_type count) const;

    size_type find_last_of(const basic_string& str, size_type pos = npos) const;
    size_type find_last_of(const_pointer s, size_type pos = npos) const;
    size_type find_last_of(value_type ch, size_type pos = npos) const;
    size_type find_last_of(const_pointer s, size_type pos, size_type count) const;
    
    size_type find_last_not_of(const basic_string& str, size_type pos = npos) const;
    size_type find_last_not_of(const_pointer s, size_type pos = npos) const;
    size_type find_last_not_of(value_type ch, size_type pos = npos) const;
    size_type find_last_not_of(const_pointer s, size_type pos, size_type count) const;

private:
    void enableCapacityToAdd(size_type addCount);
};

template <class T, class Traits, class Alloc>
const typename basic_string<T, Traits, Alloc>::size_type basic_string<T, Traits, Alloc>::npos = -1;

template <class T, class Traits, class Alloc>
void basic_string<T, Traits, Alloc>::enableCapacityToAdd(size_type addCount)
{
    if(end_ - finish_ < addCount)
    {
        const size_type oldSize = size();
        const size_type newSize = oldSize + tinystl::max(oldSize, addCount);
        iterator newStart = dataAllocator::allocate(newSize + 1);
        tinystl::uninitialized_copy(begin(), end(), newStart);
        destroy(begin(), end() + 1);
        dataAllocator::deallocate(begin());
        start_ = newStart;
        finish_ = start_ + oldSize;
        end_ = start_ + newSize;
        construct(finish_, value_type());
    }
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>::basic_string(size_type count, value_type value)
    : start_(dataAllocator::allocate(count + 1)),
      finish_(start_ + count),
      end_(finish_)
{
    tinystl::uninitialized_fill_n(start_, count, value);
    construct(finish_, value_type());
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>::basic_string(const basic_string& other, size_type pos, size_type count)
{
    if(pos > other.size())
        throw std::out_of_range("out of range");
    if(count == npos)
        count = other.size() - pos;
    start_ = dataAllocator::allocate(count + 1);
    finish_ = end_ = start_ + count;
    tinystl::uninitialized_copy(other.begin() + pos, other.begin() + pos + count, start_);
    construct(finish_, value_type());
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>::basic_string(const_pointer s, size_type count)
    : basic_string(s, s + count)
{

}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>::basic_string(const_pointer s)
    : basic_string(s, s + traits_type::length(s))
{

}

template <class T, class Traits, class Alloc>
template <class InputIterator>
basic_string<T, Traits, Alloc>::basic_string(InputIterator first, InputIterator last)
{
    auto count = last - first;
    start_ = (dataAllocator::allocate(count + 1));
    finish_ = start_ + count;
    end_ = finish_;
    tinystl::uninitialized_copy(first, last, start_);

    construct(finish_, value_type());
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>::basic_string(const basic_string& other)
    : basic_string(other.begin(), other.end())
{

}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>::basic_string(basic_string&& other) noexcept
    : start_(std::move(other.start_)),
      finish_(std::move(other.finish_)),
      end_(std::move(other.end_))
{
    other.start_ = other.finish_ = other.end_ = nullptr;
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>::basic_string(std::initializer_list<T> ilist)
    : basic_string(ilist.begin(), ilist.end())
{

}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>::~basic_string()
{
    tinystl::destroy(start_, finish_ + 1);
    dataAllocator::deallocate(start_);
}


template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::reference
basic_string<T, Traits, Alloc>::at(size_type pos)
{
    if(!(pos < size()))
        throw std::out_of_range("out of range");
    return *(begin() + pos);
}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::const_reference
basic_string<T, Traits, Alloc>::at(size_type pos) const
{
    if(!(pos < size()))
        throw std::out_of_range("out of range");
    return *(begin() + pos);
}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::reference
basic_string<T, Traits, Alloc>::operator[](size_type pos)
{
    return *(begin() + pos);
}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::const_reference
basic_string<T, Traits, Alloc>::operator[](size_type pos) const
{
    return *(begin() + pos);
}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::const_pointer
basic_string<T, Traits, Alloc>::c_str() const
{
    /* enableCapacityToAdd(1); */
    /* construct(finish_, value_type()); */
    return start_;
}

template <class T, class Traits, class Alloc>
void basic_string<T, Traits, Alloc>::reserve(size_type newCap) 
{
    if(newCap > capacity())
    {
        enableCapacityToAdd(newCap - size());
    }
    else if(newCap < size())
    {
        iterator newFinish = start_ + newCap;
        tinystl::destroy(newFinish, finish_ + 1);
        finish_ = newFinish;
        shrink_to_fit();
    }
}


template <class T, class Traits, class Alloc>
void basic_string<T, Traits, Alloc>::shrink_to_fit()
{
    const size_type newSize = size();
    iterator newStart = dataAllocator::allocate(newSize + 1);
    tinystl::uninitialized_copy(begin(), end(), newStart);
    tinystl::destroy(begin(), end() + 1);
    dataAllocator::deallocate(begin());
    start_ = newStart;
    finish_ = start_ + newSize;
    end_ = finish_;
    construct(finish_, value_type());
}


template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>&
basic_string<T, Traits, Alloc>::insert(size_type index, size_type count, value_type ch)
{
    insert(begin() + index, count, ch);
    return *this;
}
template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>&
basic_string<T, Traits, Alloc>::insert(size_type index, const_pointer s, size_type count)
{
    insert(begin() + index, s, s + count);
    return *this;
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>&
basic_string<T, Traits, Alloc>::insert(size_type index, const basic_string<T, Traits, Alloc>& str, 
                                       size_type index_str, size_type count)
{
    insert(begin() + index, str.begin() + index_str, str.begin() + index_str + count);
    return *this;
}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::iterator
basic_string<T, Traits, Alloc>::insert(const_iterator pos, size_type count, value_type ch)
{
    const size_type prevSize = tinystl::distance(begin(), pos);
    if(end_ - finish_ < count)
    {
        const size_type oldSize = size();
        const size_type newSize = oldSize + tinystl::max(oldSize, count);
        iterator newStart = dataAllocator::allocate(newSize + 1);
        auto it = tinystl::uninitialized_copy(begin(), begin() + prevSize, newStart);
        it = tinystl::uninitialized_fill_n(it, count, ch);
        tinystl::uninitialized_copy(pos, end(), it);
        tinystl::destroy(begin(), end() + 1);
        dataAllocator::deallocate(begin());
        start_ = newStart;
        finish_ = start_ + oldSize + count;
        end_ = start_ + newSize;
        construct(finish_, value_type());
    }
    else
    {
        if(finish_ - pos > count)
        {
            tinystl::destroy(end());
            auto it = tinystl::uninitialized_copy(end() - count, end(), end());
            tinystl::copy_backward(pos, end() - count, end());
            std::fill_n(pos, count, ch);
            finish_ += count;
            construct(finish_, value_type());
        }
        else
        {
            tinystl::destroy(end());
            tinystl::uninitialized_fill(end(), pos + count, value_type());
            tinystl::uninitialized_copy(pos, end(), pos + count);
            std::fill_n(pos, count, ch);
            finish_ += count; 
            construct(finish_, value_type());
        }
    }
    return begin() + prevSize;
}


template <class T, class Traits, class Alloc>
template <class InputIterator>
typename basic_string<T, Traits, Alloc>::iterator
basic_string<T, Traits, Alloc>::insert(const_iterator pos, InputIterator first, InputIterator last)
{
    const size_type count = tinystl::distance(first, last);
    const size_type prevSize = tinystl::distance(begin(), pos);
    if(end_ - finish_ < count)
    {
        const size_type oldSize = size();
        const size_type newSize = oldSize + tinystl::max(oldSize, count);
        iterator newStart = dataAllocator::allocate(newSize + 1);
        auto it = tinystl::uninitialized_copy(begin(), pos, newStart);
        it = tinystl::uninitialized_copy(first, last, it);
        tinystl::uninitialized_copy(pos, end(), it);
        tinystl::destroy(begin(), end() + 1);
        dataAllocator::deallocate(begin());
        start_ = newStart;
        finish_ = start_ + oldSize + count;
        end_ = start_ + newSize;
        construct(finish_, value_type());
    }
    else
    {
        if(finish_ - pos > count)
        {
            tinystl::destroy(end());
            auto it = tinystl::uninitialized_copy(end() - count, end(), end());
            tinystl::copy_backward(pos, end() - count, end());
            tinystl::copy(first, last, pos);
            finish_ += count;
            construct(finish_, value_type());
        }
        else
        {
            tinystl::destroy(end());
            tinystl::uninitialized_fill(end(), pos + count, value_type());
            tinystl::uninitialized_copy(pos, end(), pos + count);
            tinystl::copy(first, last, pos);
            finish_ += count; 
            construct(finish_, value_type());
        }
    }
    return begin() + prevSize;
}



template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::iterator
basic_string<T, Traits, Alloc>::insert(const_iterator pos, value_type ch)
{
    return insert(pos, 1, ch);
}
  

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::iterator
basic_string<T, Traits, Alloc>::insert(const_iterator pos, std::initializer_list<T> ilist)
{
    return insert(pos, ilist.begin(), ilist.end());
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>&
basic_string<T, Traits, Alloc>::erase(size_type index, size_type count)
{
    if(index > size())
        throw std::out_of_range("out of range");
    if(count == npos)
        count = size() - index;
    const size_type lastSize = size() - index - count;
    tinystl::copy(begin() + index + count, end(), begin() + index);
    tinystl::destroy(end() - lastSize, end());
    finish_ -= count;
    construct(finish_, value_type());
    return *this;
}
template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::iterator
basic_string<T, Traits, Alloc>::erase(const_iterator first, const_iterator last)
{
    const size_type prevSize = tinystl::distance(begin(), first);
    const size_type count = tinystl::distance(first, last);
    tinystl::copy(last, end(), first);
    tinystl::destroy(end() - count, end() + 1);
    finish_ -= count; 
    construct(finish_, value_type());
    return begin() + prevSize;
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>&
basic_string<T, Traits, Alloc>::append(size_type count, value_type ch)
{
    insert(end(), count, ch);
    return *this;
}


template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>&
basic_string<T, Traits, Alloc>::append(const basic_string& str)
{
    insert(end(), str.begin(), str.end());
    return *this;
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>&
basic_string<T, Traits, Alloc>::append(const basic_string& str,
                                       size_type pos,
                                       size_type count)
{
    if(pos >= str.size())
        throw std::out_of_range("out of range");
    if(count == npos || pos + count > str.size())
        count = str.size() - pos;
    insert(end(), str.begin() + pos, str.begin() + pos + count);
    return *this;
}


template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>&
basic_string<T, Traits, Alloc>::append(const_pointer s, size_type count)
{
    insert(end(), (s), (s) + count); 
    return *this;
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>&
basic_string<T, Traits, Alloc>::append(const_pointer s)
{
    return append(s, traits_type::length(s));
}



template <class T, class Traits, class Alloc>
template <class InputIterator>
basic_string<T, Traits, Alloc>&
basic_string<T, Traits, Alloc>::append(InputIterator first, InputIterator last)
{
    insert(end(), first, last); 
    return *this;
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>&
basic_string<T, Traits, Alloc>::append(std::initializer_list<T> ilist)
{
    insert(end(), ilist.begin(), ilist.end());
    return *this;
}

template <class T, class Traits, class Alloc>
int basic_string<T, Traits, Alloc>::compare(size_type pos1, size_type count1,
                                            const basic_string& str,
                                            size_type pos2, size_type count2) const
{
    if(count2 == npos)
        count2 = str.size() - pos2;
    return compare(pos1, count1, str.c_str() + pos2, count2);
}


template <class T, class Traits, class Alloc>
int basic_string<T, Traits, Alloc>::compare(size_type pos1, size_type count1,
                                            const_pointer s, size_type count2) const
{
    size_type count = tinystl::min(count1, count2);
    int compRes = traits_type::compare(c_str() + pos1, s,  count);
    if(compRes == 0)
    {
        if(count1 > count2)
            return 1;
        else if(count1 < count2)
            return -1;
        else
            return  0;
    }
    else
    {
        return compRes;
    }
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>&
basic_string<T, Traits, Alloc>::replace(size_type pos, size_type count, 
                                        const basic_string& str,
                                        size_type pos2, size_type count2)
{
    if(count2 == npos)
        count2 = str.size() - pos2;
    if(count == npos)
        count = size() - pos;
    return replace(begin() + pos, begin() + pos + count, str.begin() + pos2, str.begin() + pos2 + count2);
}

template <class T, class Traits, class Alloc>
template <class InputIterator>
basic_string<T, Traits, Alloc>&
basic_string<T, Traits, Alloc>::replace(const_iterator first, const_iterator last,
                                        InputIterator first2, InputIterator last2)
{
    const size_type prevSize = tinystl::distance(begin(), first);
    erase(first, last);
    insert(begin() + prevSize, first2, last2);
}


template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>&
basic_string<T, Traits, Alloc>::replace(const_iterator first, const_iterator last, 
                                        size_type count2, value_type ch)
{
    const size_type prevSize = tinystl::distance(begin(), first);
    erase(first, last);
    insert(begin() + prevSize, count2, ch);
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>
basic_string<T, Traits, Alloc>::substr(size_type pos, size_type count) const
{
    if(count == npos)
        count = size() - pos;
    if(pos > size())
        throw std::out_of_range("out of range");
    return basic_string(begin() + pos, begin() + pos + count);
}



template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::copy(pointer dest, size_type count, size_type pos) const
{
    if(pos > size())
        throw std::out_of_range("out of range");
    if(count == npos)
        count = size() - pos;
    traits_type::copy(dest, data() + pos, count);
    return count;
}


template <class T, class Traits, class Alloc>
void basic_string<T, Traits, Alloc>::resize(size_type count, value_type ch)
{
    if(count < size())
    {
        erase(begin() + count, end());
    }
    else
    {
        enableCapacityToAdd(count);
        insert(end(), count - size(), ch); 
    }
}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find(const_pointer s, size_type pos, size_type count) const
{
    vector<size_type> prefix(count, 0);
    size_type lp = 0;
    for(size_type i = 1; i < count; ++i)
    {
        while(lp > 0 && !traits_type::eq(s[i], s[lp]))
            lp = prefix[lp - 1];
        if(traits_type::eq(s[i], s[lp]))
            ++lp;
        prefix[i] = lp; 
    }
    lp = 0;
    for(size_type i = pos; i < size(); ++i)
    {
        while(lp > 0 && !traits_type::eq(*(begin() + i), s[lp]))
            lp = prefix[lp - 1];
        if((traits_type::eq(*(begin() + i), s[lp])))
            ++lp;
        if(lp == count)
            return i - count + 1;
    }
    return npos;
}


template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::rfind(const basic_string& str, size_type pos) const
{
    return rfind(str.data(), pos);

}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::rfind(const_pointer s, size_type pos) const
{
    return rfind(s, pos, traits_type::length(s));
}
template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::rfind(value_type ch, size_type pos) const
{
    if(pos == npos)
        pos = size() - 1;
    iterator it = begin() + pos;
    while(it >= begin())
    {
        if(traits_type::eq(*it, ch))
            return it - begin();
    }
    return npos;
}
template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::rfind(const_pointer s, size_type pos, size_type count) const
{
    if(pos == npos)
        pos = size() - 1;
    size_type rePos = size() - pos - 1;
    basic_string reThis(rbegin(), rend());
    basic_string tmp(s, s + count);
    basic_string reStr(tmp.rbegin(), tmp.rend());
    size_type res = reThis.find(reStr, rePos);
    if(res != npos)
        res =  size() - res - count;
    return res;
}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_first_of(const basic_string& str, size_type pos) const
{
    return find_first_of(str.data(), pos);

}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_first_of(const_pointer s, size_type pos) const
{
    return find_first_of(s, pos, traits_type::length(s));
}


template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_first_of(value_type ch, size_type pos) const
{
    return find_first_of(&ch, pos, 1);
}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_first_of(const_pointer s, size_type pos, size_type count) const
{
    for(iterator it = begin() + pos; it != end(); ++it)
    {
        if(traits_type::find(s, count, *it) != nullptr)
            return it - begin();
    }
    return npos;
}



template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_first_not_of(const basic_string& str, size_type pos) const
{
    return find_first_not_of(str.data(), pos);

}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_first_not_of(const_pointer s, size_type pos) const
{
    return find_first_not_of(s, pos, traits_type::length(s));
}


template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_first_not_of(value_type ch, size_type pos) const
{
    return find_first_not_of(&ch, pos, 1);
}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_first_not_of(const_pointer s, size_type pos, size_type count) const
{
    for(iterator it = begin() + pos; it != end(); ++it)
    {
        if(traits_type::find(s, count, *it) == nullptr)
            return it - begin();
    }
    return npos;
}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_last_of(const basic_string& str, size_type pos) const
{
    return find_last_of(str.data(), pos);

}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_last_of(const_pointer s, size_type pos) const
{
    return find_last_of(s, pos, traits_type::length(s));
}


template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_last_of(value_type ch, size_type pos) const
{
    return find_last_of(&ch, pos, 1);
}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_last_of(const_pointer s, size_type pos, size_type count) const
{
    if(pos == npos)
        pos = size() - 1;
    for(auto it = begin() + pos; it >= begin(); --it)
    {
        if(traits_type::find(s, count, *it) != nullptr)
            return it - begin();
    }
    return npos;
}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_last_not_of(const basic_string& str, size_type pos) const
{
    return find_last_not_of(str.data(), pos);

}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_last_not_of(const_pointer s, size_type pos) const
{
    return find_last_not_of(s, pos, traits_type::length(s));
}


template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_last_not_of(value_type ch, size_type pos) const
{
    return find_last_not_of(&ch, pos, 1);
}

template <class T, class Traits, class Alloc>
typename basic_string<T, Traits, Alloc>::size_type
basic_string<T, Traits, Alloc>::find_last_not_of(const_pointer s, size_type pos, size_type count) const
{
    if(pos == npos)
        pos = size() - 1;
    for(auto it = begin() + pos; it >= begin(); --it)
    {
        if(traits_type::find(s, count, *it) == nullptr)
            return it - begin();
    }
    return npos;
}


/* ****************************运算符重载************************************** */

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>
operator+(const basic_string<T, Traits, Alloc>& lhs,
         const basic_string<T, Traits, Alloc>& rhs)
{
    auto res(lhs);
    res.append(rhs);
    return res;
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>
operator+(const T* lhs,
          const basic_string<T, Traits, Alloc>& rhs)
{
    basic_string<T, Traits, Alloc> res(lhs);
    res.append(rhs);
    return res;
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>
operator+(const basic_string<T, Traits, Alloc>& lhs,
          const T* rhs)
{
    auto res(lhs);
    res.append(rhs);
    return res;
}

template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>
operator+(const basic_string<T, Traits, Alloc>& lhs,
          T rhs)
{
    auto res(lhs);
    res.push_back(rhs);
    return res;
}


template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc>
operator+(const T lhs,
         const basic_string<T, Traits, Alloc>& rhs)
{
    basic_string<T, Traits, Alloc> res(1, lhs);
    res.append(rhs);
    return res;
}

namespace
{
    template <class InputIterator1, class InputIterator2>
    bool rangeEqual(InputIterator1 first1, InputIterator1 last1,
                   InputIterator2 first2, InputIterator2 last2)
         
    {
        while(first1 != last1 && first2 != last2)
        {
            if(*first1 != *first2)
                return false;
            ++first1;
            ++first2;
        }
        return first1 == last1 && first2 == last2;
    }
}


template <class T, class Traits, class Alloc>
bool operator==(const basic_string<T, Traits, Alloc>& lhs,
                const basic_string<T, Traits, Alloc>& rhs)
{
    return rangeEqual(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T, class Traits, class Alloc>
bool operator==(const basic_string<T, Traits, Alloc>& lhs,
                const T* rhs)
{
    return rangeEqual(lhs.begin(), lhs.end(), rhs, rhs + Traits::length(rhs));
}

template <class T, class Traits, class Alloc>
bool operator==(const T* lhs,
                const basic_string<T, Traits, Alloc>& rhs)
{
    return rhs == lhs;
}


template <class T, class Traits, class Alloc>
bool operator!=(const basic_string<T, Traits, Alloc>& lhs,
                const basic_string<T, Traits, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <class T, class Traits, class Alloc>
bool operator!=(const basic_string<T, Traits, Alloc>& lhs,
                const T* rhs)
{
    return !(lhs == rhs);
}

template <class T, class Traits, class Alloc>
bool operator!=(const T* lhs,
                const basic_string<T, Traits, Alloc>& rhs)
{
    return !(lhs == rhs);
}


template <class T, class Traits, class Alloc>
bool operator>(const basic_string<T, Traits, Alloc>& lhs,
                const basic_string<T, Traits, Alloc>& rhs)
{
    return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

template <class T, class Traits, class Alloc>
bool operator>(const basic_string<T, Traits, Alloc>& lhs,
                const T* rhs)
{
    return std::lexicographical_compare(rhs, rhs + Traits::length(rhs), lhs.begin(), lhs.end());
}

template <class T, class Traits, class Alloc>
bool operator>(const T* lhs,
                const basic_string<T, Traits, Alloc>& rhs)
{
    return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs, lhs + Traits::length(lhs));
}

template <class T, class Traits, class Alloc>
bool operator<(const basic_string<T, Traits, Alloc>& lhs,
                const basic_string<T, Traits, Alloc>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T, class Traits, class Alloc>
bool operator<(const basic_string<T, Traits, Alloc>& lhs,
                const T* rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs, rhs + Traits::length(rhs));
}

template <class T, class Traits, class Alloc>
bool operator<(const T* lhs,
                const basic_string<T, Traits, Alloc>& rhs)
{
    return std::lexicographical_compare(lhs, lhs + Traits::length(lhs), rhs.begin(), rhs.end());
}

template <class T, class Traits, class Alloc>
bool operator<=(const basic_string<T, Traits, Alloc>& lhs,
                const basic_string<T, Traits, Alloc>& rhs)
{
    return !(lhs > rhs);
}

template <class T, class Traits, class Alloc>
bool operator<=(const basic_string<T, Traits, Alloc>& lhs,
                const T* rhs)
{
    return !(lhs > rhs);
}

template <class T, class Traits, class Alloc>
bool operator<=(const T* lhs,
                const basic_string<T, Traits, Alloc>& rhs)
{
    return !(lhs > rhs);
}

template <class T, class Traits, class Alloc>
bool operator>=(const basic_string<T, Traits, Alloc>& lhs,
                const basic_string<T, Traits, Alloc>& rhs)
{
    return !(lhs < rhs);
}

template <class T, class Traits, class Alloc>
bool operator>=(const basic_string<T, Traits, Alloc>& lhs,
                const T* rhs)
{
    return !(lhs < rhs);
}

template <class T, class Traits, class Alloc>
bool operator>=(const T* lhs,
                const basic_string<T, Traits, Alloc>& rhs)
{
    return !(lhs < rhs);
}

template <class T, class Traits, class Alloc>
std::basic_istream<T>& operator>>(std::basic_istream<T>& is, basic_string<T, Traits, Alloc>& str)
{
    str.clear();
    T ch;
    while((ch = is.get()))
    {
        if(std::isblank(ch) || Traits::eq(ch, '\n')) 
            continue;
        break;
    }
    str.push_back(ch);
    while((ch = is.get()))
    {
        if(std::isblank(ch) || ch == EOF || Traits::eq(ch, '\n'))
            break;
        str.push_back(ch);
    }
    return is;
}

template <class T, class Traits, class Alloc>
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, basic_string<T, Traits, Alloc>& str)
{
    for(auto& ch : str)
        os << ch;
    return os;
}

template <class T, class Traits, class Alloc>
std::basic_ostream<T>& operator<<(std::basic_ostream<T> os, const basic_string<T, Traits, Alloc>& str)
{
    for(auto& ch : str)
        os << ch;
    return os;
}
template <class T, class Traits, class Alloc>
void swap(basic_string<T, Traits, Alloc>& lhs,
          basic_string<T, Traits, Alloc>& rhs)
{
    lhs.swap(rhs);
}


}
