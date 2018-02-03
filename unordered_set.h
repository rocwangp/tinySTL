#pragma once

#include "utility.h"
#include "hashtable.h"

namespace tinystl
{

template <class Key, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>, class Alloc = alloc>
class unordered_set
{
public:
    typedef Key                         key_type;
    typedef Key                         value_type;
    typedef std::size_t                 size_type;
    typedef std::ptrdiff_t              difference_type;
    typedef Hash                        hasher;
    typedef KeyEqual                    key_equal;
    typedef Alloc                       allocator_type;
    typedef value_type&                 reference;
    typedef const value_type&           const_reference;
    typedef value_type*                 pointer;
    typedef const value_type*           const_pointer;
    typedef HashTable<key_type, value_type, 
                      std::_Identity<value_type>, 
                      Hash, KeyEqual, Alloc>  container_type;
    typedef typename container_type::iterator           iterator;
    typedef typename container_type::const_iterator     const_iterator;
    typedef typename container_type::local_iterator     local_iterator;
    typedef typename container_type::const_local_iterator const_local_iterator;

    typedef unordered_set       Self;

    enum { INIT_BUCKET_SIZE = 128 };
    container_type cont_;

public:
    explicit unordered_set(size_type bucketCount = INIT_BUCKET_SIZE, 
                           const hasher& hash = hasher(),
                           const key_equal& equal = key_equal())
        : cont_(bucketCount, hash, equal)
    {  }
    
    template <class InputIterator>
    unordered_set(InputIterator first ,InputIterator last, 
                  size_type bucketCount = INIT_BUCKET_SIZE,
                  const hasher& hash = hasher(),
                  const key_equal& equal = key_equal())
        : cont_(bucketCount, hash, equal)
    {
        while(first != last)
            insert(*first++);
    }

    unordered_set(const Self& other)
        : cont_(other.cont_)
    {  }

    unordered_set(Self&& other)
        : cont_(std::move(other.cont_))
    {  }

    ~unordered_set() {}

    Self& operator=(const Self& other) { cont_ = other.cont_; return *this; }
    Self& operator=(Self&& other) { cont_ = std::move(other.cont_); return *this; }

    iterator begin() noexcept { return cont_.begin(); }
    const_iterator begin() const noexcept { return cont_.begin(); }
    const_iterator cbegin() const noexcept { return cont_.cbegin(); }

    iterator end() noexcept { return cont_.end(); }
    const_iterator end() const noexcept { return cont_.end(); }
    const_iterator cend() const noexcept { return cont_.cend(); }

    bool empty() const noexcept { return cont_.empty(); }
    size_type size() const noexcept { return cont_.size(); }
    size_type max_size() const noexcept { return cont_.max_size(); }

    void clear() { cont_.clear(); }

    tinystl::pair<iterator, bool> insert(const value_type& value)
    {
        iterator it = cont_.insert(value);
        if(it == end())
            return tinystl::make_pair(it, false);
        else
            return tinystl::make_pair(it, true);
    }

    template <class InputIterator>
    void insert(InputIterator first ,InputIterator last)
    {
        while(first != last)
            insert(*first++);
    }


    iterator erase(const_iterator pos)
    {
        return cont_.erase(pos);
    }

    size_type erase(const key_type& key)
    {
        return cont_.erase(key);
    }

    void swap(Self& other)
    {
        cont_.swap(other.cont_);
    }
public:
    size_type count(const key_type& key) { return cont_.count(key); }
    iterator find(const key_type& key) { return cont_.find(key); }
    const_iterator find(const key_type& key) const { return cont_.find(key); }

    tinystl::pair<iterator, iterator> equal_range(const key_type& key) { return cont_.equal_range(key); }
    tinystl::pair<const_iterator, const_iterator> equal_range(const key_type& key) const { return cont_.equal_range(key); }

public:
    local_iterator begin(size_type n) { return cont_.begin(n); }
    const_local_iterator begin(size_type n) const { return cont_.begin(n); }
    const_local_iterator cbegin(size_type n) const { return cont_.cbegin(n); }

    local_iterator end(size_type n) { return cont_.end(n); }
    const_local_iterator end(size_type n) const { return cont_.end(n); }
    const_local_iterator cend(size_type n) const { return cont_.cend(n); }

    size_type bucket_count() const { return cont_.bucket_count(); }
    size_type max_bucket_count() const { return cont_.max_bucket_count(); }
    size_type bucket_size(size_type n) const { return cont_.bucket_size(n); }
    size_type bucket(const key_type& key) const  { return cont_.bucket(key); }

public:
    float load_factor() const { return cont_.load_factor(); }
    float max_load_factor() const { return cont_.max_load_factor(); }
    void max_load_factor(float ml) { cont_.max_load_factor(ml); }

    void rehash(size_type count) { cont_.rehash(count); }
    void reserve(size_type count) { cont_.reserve(count); }

public:
    hasher hash_function() const { return cont_.hash_function(); }
    key_equal key_eq() const { return cont_.key_eq(); }
};

template <class Key, class Hash, class KeyEqual, class Alloc>
void swap(const HashTable<Key, Hash, KeyEqual, Alloc>& lhs,
          const HashTable<Key, Hash, KeyEqual, Alloc>& rhs)
{
    lhs.swap(rhs);
}


}
