#pragma once

#include "skiplist.h"

namespace tinystl
{


template <class Key, class Compare = std::less<Key>, class Alloc = alloc>
class set
{
public:
    typedef Key             key_type;
    typedef Key             value_type;
    typedef std::size_t     size_type;
    typedef std::ptrdiff_t  difference_type;
    typedef Compare         key_compare;
    typedef Compare         value_compare;
    typedef value_type&     reference;
    typedef const value_type& const_reference;
    typedef value_type*     pointer;
    typedef const value_type* const_pointer;

    typedef SkipList<Key, Key, std::_Identity<Key>, Compare, Alloc>          container_type;
    typedef typename container_type::iterator           iterator;
    typedef typename container_type::const_iterator     const_iterator;
    typedef typename container_type::reverse_iterator   reverse_iterator;
    typedef typename container_type::const_reverse_iterator const_reverse_iterator;

protected:
    container_type cont_;

public:
    explicit set(const Compare& comp = Compare())
        : cont_(comp)
    {  }

    template <class InputIterator>
    set(InputIterator first, InputIterator last, const Compare& comp = Compare())
        : set(comp)
    {
        while(first != last)
            cont_.insert_unique(*first++);
    }

    set(const set& other)
        : cont_(other.cont_)
    {  }

    set(set&& other)
        : cont_(std::move(other.cont_))
    {  }

    set(std::initializer_list<value_type> init, const Compare& comp = Compare())
        : cont_(init, comp)
    {  }

    ~set() {}

    set& operator=(const set& other) { cont_ = other.cont_; return *this; }
    set& operator=(set&& other) { cont_ = std::move(other.cont_); return *this; }
    set& operator=(std::initializer_list<value_type> ilist) { cont_ = ilist; return *this; }

    iterator begin() noexcept { return cont_.begin(); }
    const_iterator begin() const noexcept { return cont_.begin(); }
    const_iterator cbegin() const noexcept { return cont_.cbegin(); }
    reverse_iterator rbegin() noexcept { return cont_.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return cont_.rbegin(); }
    const_reverse_iterator crbegin() const noexcept { return cont_.crbegin(); }

    iterator end() noexcept { return cont_.end(); }
    const_iterator end() const noexcept { return cont_.end(); }
    const_iterator cend() const noexcept { return cont_.cend(); }
    reverse_iterator rend() noexcept { return cont_.rend(); }
    const_reverse_iterator rend() const noexcept { return cont_.rend(); }
    const_reverse_iterator crend() const noexcept { return cont_.crend(); }

    bool empty() const noexcept { return cont_.empty(); }
    size_type size() const noexcept { return cont_.size(); }
    size_type max_size() const noexcept { return cont_.max_size(); }

    void clear() noexcept { cont_.clear(); }

    std::pair<iterator, bool> insert(const value_type& value)
    {
        iterator it = cont_.insert_unique(value);
        if(it == end())
            return std::make_pair(it, false);
        else
            return std::make_pair(it, true);
    }

    template <class InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        while(first != last)
            cont_.insert_unique(*first++);
    }

    void insert(std::initializer_list<value_type> ilist)
    {
        insert(ilist.begin(), ilist.end());
    }

    size_type erase(const key_type& key) { return cont_.erase(key); }

    size_type count(const key_type& key) const { return cont_.count(key); }
    iterator find(const key_type& key) { return cont_.find(key); }
    const_iterator find(const key_type& key) const { return cont_.find(key); }

    std::pair<iterator, iterator> equal_range(const key_type& key) { return cont_.equal_range(key); }
    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const { return cont_.equal_range(key); }

    iterator lower_bound(const key_type& key) { return cont_.lower_bound(key); }
    const_iterator lower_bound(const key_type& key) const { return cont_.lower_bound(key); }

    iterator upper_bound(const key_type& key) { return cont_.upper_bound(key); }
    const_iterator upper_bound(const key_type& key) const { return cont_.upper_bound(key); }

public:
    const container_type& container() const { return cont_; }
    key_compare key_comp() const { return cont_.key_comp(); }
    value_compare value_comp() const { return cont_.value_comp(); }

    void swap(set& other) { cont_.swap(other.cont_); }
};    


template <class Key, class Compare,  class Alloc>
bool operator==(const set<Key, Compare, Alloc>& lhs,
                const set<Key, Compare, Alloc>& rhs)
{
    return lhs.container() == rhs.container();
}

template <class Key, class Compare,  class Alloc>
bool operator!=(const set<Key, Compare, Alloc>& lhs,
                const set<Key, Compare, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <class Key, class Compare,  class Alloc>
bool operator<(const set<Key, Compare, Alloc>& lhs,
                const set<Key, Compare, Alloc>& rhs)
{
    return lhs.container() < rhs.container();
}

template <class Key, class Compare,  class Alloc>
bool operator>(const set<Key, Compare, Alloc>& lhs,
                const set<Key, Compare, Alloc>& rhs)
{
    return lhs.container() > rhs.container();
}

template <class Key, class Compare,  class Alloc>
bool operator<=(const set<Key, Compare, Alloc>& lhs,
                const set<Key, Compare, Alloc>& rhs)
{
    return !(lhs > rhs);
}


template <class Key, class Compare,  class Alloc>
bool operator>=(const set<Key, Compare, Alloc>& lhs,
                const set<Key, Compare, Alloc>& rhs)
{
    return !(lhs < rhs);
}

template <class Key, class Compare, class Alloc>
void swap(set<Key, Compare, Alloc>& lhs,
          set<Key, Compare, Alloc>& rhs)
{
    lhs.swap(rhs);
}

}
