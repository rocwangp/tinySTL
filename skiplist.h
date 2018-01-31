#pragma once

#include "alloc.h"
#include "iterator.h"
#include "vector.h"

#include <memory>
#include <algorithm>
#include <random>
#include <vector>

namespace tinystl
{


template <class T, class Alloc>
class SkipListNode
{
public:

    typedef T            value_type;
    typedef SkipListNode node_type;
    typedef node_type*   node_pointer;
    typedef std::size_t  size_type;

    class LevelNode
    { 
    public:
        LevelNode() : next(nullptr), steps(0) {}
        node_pointer next;
        int steps;
    };
    typedef LevelNode level_type;
    typedef level_type*      level_pointer;

    typedef SimpleAlloc<level_type, Alloc>  levelAllocator;

    SkipListNode(const value_type& v, 
                 size_type l = 1,
                 node_pointer p = nullptr)
        : value(v),
          parent(p),
          levelNums(l)
    {
        levels = levelAllocator::allocate(levelNums);
        std::uninitialized_fill_n(levels, levelNums, level_type());
    }
    ~SkipListNode()
    {
        destroy(levels, levels + levelNums);
        levelAllocator::deallocate(levels);
    }
    value_type value;
    node_pointer parent;
    level_pointer levels; 
    size_type levelNums;
};


template <class T, class Alloc>
class SkipListIterator
{
public:
    typedef bidirectional_iterator_tag      iterator_category;
    typedef SkipListNode<T, Alloc>          node_type;
    typedef node_type*                      node_pointer; 
    typedef T                               value_type;
    typedef T&                              reference;
    typedef const T&                        const_reference;
    typedef T*                              pointer;
    typedef const pointer                   const_pointer;
    
    typedef std::ptrdiff_t                  difference_type;
    typedef std::size_t                     size_type;
    typedef SkipListIterator<T, Alloc>      Self;
public:
    node_pointer node_;

public:
    SkipListIterator(node_pointer node) : node_(node) {}
    SkipListIterator(const Self& other) : node_(other.node_) {}
    Self& operator=(const Self& other) { node_ = other.node_; return *this; }
    reference operator*()
    {
        return node_->value;
    }
    const_reference operator*() const
    {
        return node_->value;
    }
    pointer operator->()
    {
        return &node_->value;
    }
    
    Self& operator++()
    {
        node_ = node_->levels[0].next;
        return *this;
    }
    Self& operator--()
    {
        node_ = node_->parent;
        return *this;
    }
    Self operator++(int)
    {
        Self tmp(*this);
        ++(*this);
        return tmp;
    }
    Self operator--(int)
    {
        Self tmp(*this);
        --(*this);
        return tmp;
    }

    bool operator==(const Self& other) const 
    {
        return node_ == other.node_;
    }

    bool operator!=(const Self& other) const
    {
        return node_ != other.node_;
    }
};


template <class Key, class Value, class KeyOfValue, class Compare = std::less<Key>, class Alloc = alloc>
class SkipList
{
public:
    typedef Key         key_type;
    typedef Value       value_type;
    typedef Compare     key_compare;
    typedef Compare     value_compare;

    typedef SkipListNode<Value, Alloc>      node_type;
    typedef node_type*                      node_pointer;
    typedef typename node_type::level_type  level_type;
    typedef SkipListIterator<Value, Alloc>  iterator;
    typedef const iterator                  const_iterator;
    typedef tinystl::reverse_iterator<iterator> reverse_iterator;
    typedef const reverse_iterator          const_reverse_iterator;

    typedef std::size_t                     size_type;
    
    typedef SimpleAlloc<node_type, Alloc>   nodeAllocator;
    typedef SimpleAlloc<level_type, Alloc>  levelAllocator;

    typedef SkipList                        Self;
public:
    enum { MAX_LEVEL_NUMS = 32 };
    enum class InPattern { UNIQUE, MULTI };
public:
    node_pointer header_;
    node_pointer tail_;
    key_compare comp_;
    size_type maxLevels_; 
    
private:
    static double kLevelDP;
public:
    SkipList(const Compare& comp = Compare()) 
        : header_(constructNode(value_type(), MAX_LEVEL_NUMS, nullptr)),
          tail_(constructNode(value_type(), MAX_LEVEL_NUMS, header_)),
          comp_(comp),
          maxLevels_(MAX_LEVEL_NUMS)
    {
        for(size_type i = 0; i < maxLevels_; ++i)
            header_->levels[i].next = tail_;
    }

    template <class InputIterator>
    SkipList(InputIterator first, InputIterator last, const Compare& comp = Compare())
        : SkipList(comp)
    {
        while(first != last)
            insert_unique(*first++);
    }
        
    SkipList(std::initializer_list<value_type> init, const Compare& comp = Compare())
        : SkipList(init.begin(), init.end(), comp)
    {  

    }

    ~SkipList() 
    {
        clear();
        destroy(header_);
        destroy(tail_);
    }

    SkipList(const Self& other);
    SkipList(Self&& other) noexcept;
    Self& operator=(const SkipList& other) { Self tmp(other); this->swap(tmp); return *this; }
    Self& operator=(SkipList&& other) { Self tmp(std::move(other)); this->swap(tmp); return *this; }

    void print();

    bool empty() const noexcept { return begin() == end(); }
    size_type size() const noexcept { iterator b = begin(); iterator e = end(); return tinystl::distance(begin(), end()) ; }
    size_type max_size() const noexcept { return static_cast<size_type>(-1); }

    iterator begin() noexcept  { return iterator(header_->levels[0].next); }
    iterator end() noexcept { return iterator(tail_); }

    const_iterator begin() const noexcept { return static_cast<const_iterator>(iterator(header_->levels[0].next)); }
    const_iterator end() const noexcept { return static_cast<const_iterator>(iterator(tail_)); }

    const_iterator cbegin() const noexcept { return const_iterator(header_->levles[0].next); }
    const_iterator cend() const noexcept { return const_iterator(tail_); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(tail_); }
    reverse_iterator rend() noexcept { return reverse_iterator(header_->levels[0].next); }
    
    iterator insert_unique(const value_type& value) { return insertAux(value, InPattern::UNIQUE); }
    iterator insert_multi(const value_type& value) { return insertAux(value, InPattern::MULTI); }

    size_type erase(const value_type& value);
    void clear();
    
    size_type count(const key_type& key) const;
    iterator find(const key_type& key) ;
    const_iterator find(const key_type& key) const { return const_cast<SkipList*>(this)->find(key); }

    std::pair<iterator, iterator> equal_range(const key_type& key);
    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;
    iterator lower_bound(const key_type& key) { return equal_range(key).first; }
    const_iterator lower_bound(const key_type& key) const { return equal_range(key).first; }
    iterator upper_bound(const key_type& key) { return equal_range(key).second; }
    const_iterator upper_bound(const key_type& key) const { return equal_range(key).second; }

    void swap(Self& other)
    {
        std::swap(header_, other.header_);
        std::swap(tail_, other.tail_);
        std::swap(maxLevels_, other.maxLevels_);
    }
public:
    key_compare key_comp() const { return  key_compare(); }
    value_compare value_comp() const { return value_compare(); }
private:
    iterator insertAux(const value_type& value, InPattern pattern);
private:
    node_pointer constructNode(const value_type& value, size_type levels = MAX_LEVEL_NUMS, node_pointer parent = nullptr);
    void destroyNode(node_pointer node);
    size_type randomLevel();
};


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
double SkipList<Key, Value, KeyOfValue, Compare, Alloc>::kLevelDP = 0.25;


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void SkipList<Key, Value, KeyOfValue, Compare, Alloc>::print()
{
    for(size_type i = 0; i < MAX_LEVEL_NUMS; ++i)
        std::cout << header_->levels[i].next << " ";
    std::cout << "done" << std::endl;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename SkipList<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
SkipList<Key, Value, KeyOfValue, Compare, Alloc>::constructNode(const value_type& value, size_type levels, node_pointer parent)
{
    return new node_type(value, levels, parent);
    node_pointer node = nodeAllocator::allocate(1);
    construct(node, node_type(value, levels, parent));
    return node;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void SkipList<Key, Value, KeyOfValue, Compare, Alloc>::destroyNode(node_pointer node)
{
    destroy(node);
    nodeAllocator::deallocate(node);
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
SkipList<Key, Value, KeyOfValue, Compare, Alloc>::SkipList(const SkipList& other)
    : SkipList()
{
    maxLevels_ = other.maxLevels_;
    tinystl::vector<node_pointer> lprevs(maxLevels_, header_);
    tinystl::vector<node_pointer> rprevs(other.maxLevels_, other.header_);
    for(auto it = other.begin(); it != other.end(); ++it)
    {
        node_pointer newNode = constructNode(*it, it.node_->levelNums, lprevs[0]); 
        for(size_type i = 0; i < maxLevels_; ++i)
        {
            if(rprevs[i]->levels[i].next != it.node_)
                break;
            rprevs[i] = it.node_;
            lprevs[i]->levels[i].next = newNode;
            lprevs[i] = newNode;
        }
    }
    tail_->parent = lprevs[0];
    for(size_type i = 0; i < maxLevels_; ++i)
        lprevs[i]->levels[i].next = tail_;
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
SkipList<Key, Value, KeyOfValue, Compare, Alloc>::SkipList(SkipList&& other) noexcept
    : SkipList()
{
    swap(other);
}



template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename SkipList<Key, Value, KeyOfValue, Compare, Alloc>::size_type
SkipList<Key, Value, KeyOfValue, Compare, Alloc>::randomLevel()
{
    size_type level = 1;
    std::random_device rd;
    while((rd() % 0XFFFF) < (kLevelDP * 0XFFFF))
          ++level;
    return (level < MAX_LEVEL_NUMS) ? level : MAX_LEVEL_NUMS;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename SkipList<Key, Value, KeyOfValue, Compare, Alloc>::iterator
SkipList<Key, Value, KeyOfValue, Compare, Alloc>::insertAux(const value_type& value, InPattern pattern)
{
    tinystl::vector<node_pointer> prevs(MAX_LEVEL_NUMS, header_);
    node_pointer cur = header_;
    for(int i = MAX_LEVEL_NUMS - 1; i >= 0; --i)
    {
        while(cur->levels[i].next != tail_ && comp_(KeyOfValue()(cur->levels[i].next->value), KeyOfValue()(value)))
            cur = cur->levels[i].next;
        prevs[i] = cur;
    }
    if(pattern == InPattern::UNIQUE &&
       cur->levels[0].next != tail_ && 
       !comp_(KeyOfValue()(cur->levels[0].next->value), KeyOfValue()(value)) && 
       !comp_(KeyOfValue()(value),  KeyOfValue()(cur->levels[0].next->value)))
        return iterator(nullptr);
    
    size_type level = randomLevel();
    node_pointer newNode = constructNode(value, level);
    for(int i = level - 1; i >= 0; --i)
    {
        newNode->levels[i].next = prevs[i]->levels[i].next;
        prevs[i]->levels[i].next = newNode;
    }

    newNode->parent = cur;
    newNode->levels[0].next->parent = newNode;
    return iterator(newNode);
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename SkipList<Key, Value, KeyOfValue, Compare, Alloc>::size_type
SkipList<Key, Value, KeyOfValue, Compare, Alloc>::erase(const value_type& value)
{
    tinystl::vector<node_pointer> prevs(maxLevels_);
    node_pointer cur = header_;
    for(int i = maxLevels_ - 1; i >= 0; --i)
    {
        while(cur->levels[i].next != tail_ && comp_(KeyOfValue()(cur->levels[i].next->value), KeyOfValue()(value)))
            cur = cur->levels[i].next;
        prevs[i] = cur;
    }
    
    node_pointer eraseNode = nullptr;
    for(int i = maxLevels_ - 1; i >= 0; --i)
    {
        if(prevs[i]->levels[i].next != tail_ && 
           !comp_(KeyOfValue()(value), KeyOfValue()(prevs[i]->levels[i].next->value)) &&
           !comp_(KeyOfValue()(prevs[i]->levels[i].next->value), KeyOfValue()(value)))
        {
            eraseNode = prevs[i]->levels[i].next;
            prevs[i]->levels[i].next = prevs[i]->levels[i].next->levels[i].next;
        }
    }
    if(eraseNode)
    {
        eraseNode->levels[0].next->parent = eraseNode->parent;
        destroyNode(eraseNode);
    }
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void SkipList<Key, Value, KeyOfValue, Compare, Alloc>::clear()
{
    node_pointer cur = header_->levels[0].next;
    while(cur != tail_)
    {
        node_pointer next = cur->levels[0].next;
        destroyNode(cur);
        cur = next;
    }
    for(int i = 0; i < maxLevels_; ++i)
        header_->levels[i].next = tail_;
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename SkipList<Key, Value, KeyOfValue, Compare, Alloc>::size_type
SkipList<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type& key) const
{
    size_type cnt = 0;
    node_pointer cur = header_;
    for(int i = maxLevels_ - 1; i >= 0; --i)
    {
        while(cur->levels[i].next != tail_ && comp_(KeyOfValue()(cur->levels[i].next->value), key))
            cur = cur->levels[i].next;
    }
    cur = cur->levels[0].next;
    while(cur != tail_ &&
          !comp_(KeyOfValue()(cur->value), key) &&
          !comp_(key, KeyOfValue()(cur->value)))
    {
        ++cnt;
        cur = cur->levels[0].next;
    }
    return cnt;
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename SkipList<Key, Value, KeyOfValue, Compare, Alloc>::iterator
SkipList<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& key)
{
    node_pointer cur = header_;
    for(int i = maxLevels_ - 1; i >= 0; --i)
    {
        while(cur->levels[i].next != tail_ && comp_(KeyOfValue()(cur->levels[i].next->value), key))
            cur = cur->levels[i].next;
    }
    if(cur->levels[0].next != tail_ &&
      !comp_(KeyOfValue()(cur->levels[0].next->value), key) &&
      !comp_(key, KeyOfValue()(cur->levels[0].next->value)))
    {
        return iterator(cur->levels[0].next);
    }
    else
    {
        return end();
    }
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
std::pair<typename SkipList<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
          typename SkipList<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
SkipList<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& key)
{
    node_pointer cur = header_;
    for(int i = maxLevels_ - 1; i >= 0; --i)
    {
        while(cur->levels[i].next != tail_ && comp_(KeyOfValue()(cur->levels[i].next->value), key))
            cur = cur->levels[i].next;
    }
    cur = cur->levels[0].next;
    node_pointer leftBound = cur;
    node_pointer rightBound = cur;
    if(cur != tail_ &&
       !comp_(KeyOfValue()(cur->value), key) &&
       !comp_(key, KeyOfValue()(cur->value)))
    {
        leftBound = cur;
        rightBound = cur->levels[0].next;
        while(rightBound != tail_ &&
              !comp_(KeyOfValue()(rightBound->value), key) && 
              !comp_(key, KeyOfValue()(rightBound->value)))
        {
            rightBound = rightBound->levels[0].next;
        }
    }
    return std::make_pair(iterator(leftBound), iterator(rightBound));
}
          
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
std::pair<typename SkipList<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
          typename SkipList<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
SkipList<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& key) const
{
    auto p = const_cast<SkipList*>(this)->equal_range(key);
    return std::make_pair(static_cast<const_iterator>(p.first),
                          static_cast<const_iterator>(p.second));
}
          

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
bool operator==(const SkipList<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
                const SkipList<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
{
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    while(lit != lhs.end() && rit != rhs.end())
    {
        if(*lit != *rit)
            return false;
        ++lit;
        ++rit;
    }
    return lit == lhs.end() && rit == rhs.end();
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
bool operator!=(const SkipList<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
                const SkipList<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
bool operator<(const SkipList<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
                const SkipList<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
bool operator>(const SkipList<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
                const SkipList<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
{
    return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
bool operator<=(const SkipList<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
                const SkipList<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
{
    return !(lhs > rhs);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
bool operator>=(const SkipList<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
                const SkipList<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
{
    return !(lhs < rhs);
}



}
