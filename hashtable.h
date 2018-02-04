#pragma once

#include "alloc.h"
#include "iterator.h"
#include "vector.h"
#include "list.h"
#include "utility.h"
#include "algorithm.h"



namespace tinystl
{
    
template <class T>
class HashTableIterator
{
public:
    typedef forward_iterator_tag        iterator_category;
    typedef T                           value_type;
    typedef value_type&                 reference;
    typedef const value_type&           const_reference;
    typedef value_type*                 pointer;
    typedef const value_type*           const_pointer;
    typedef std::size_t                 size_type;
    typedef std::ptrdiff_t              difference_type;

    typedef tinystl::vector<tinystl::list<value_type>> bucket_type;
    typedef typename tinystl::list<value_type>::iterator local_iterator;
    typedef HashTableIterator           Self;
public:
    int bucketIdx;
    local_iterator listIt;
private:
    bucket_type& bucket;

public:
    HashTableIterator(int idx, bucket_type& b)
        : bucketIdx(idx), bucket(b)
    {
        if(idx >= bucket.size())
        {
            idx = bucket.size();
            listIt = local_iterator();
        }
        else
        {
            listIt = bucket[idx].begin();
        }
    }

    HashTableIterator(int idx, bucket_type& b, local_iterator it)
        : bucketIdx(idx), bucket(b), listIt(it)
    {  }

    HashTableIterator(const Self& other)
        : bucketIdx(other.bucketIdx),
          listIt(other.listIt),
          bucket(other.bucket)
    {  }

    Self& operator=(const Self& other)
    {
        Self tmp(other);
        this->swap(tmp);
        return *this;
    }

    void swap(Self& other)
    {
        tinystl::swap(bucketIdx, other.bucketIdx);
        tinystl::swap(listIt, other.listIt);
        bucket.swap(other.bucket);
    }
    reference operator*()
    {
        return *listIt;
    }
    const_reference operator*() const 
    {
        return *listIt;
    }
    pointer operator->()
    {
        return &(*listIt);
    }

    Self& operator++()
    {
        local_iterator tmp(listIt);
        if(listIt == bucket[bucketIdx].end() || (++tmp) == bucket[bucketIdx].end())
        {
            ++bucketIdx;
            while(bucketIdx < bucket.size() && bucket[bucketIdx].empty())
                ++bucketIdx;
            if(bucketIdx < bucket.size())
                listIt = bucket[bucketIdx].begin();
            else
                listIt = local_iterator(nullptr);
        }
        else
        {
            ++listIt;
        }
        return *this;
    }
    Self operator++(int)
    {
        Self tmp(*this);
        ++(*this);
        return tmp;
    }

    bool operator==(const Self& other)
    {
        return bucketIdx == other.bucketIdx && listIt == other.listIt && (&bucket == &other.bucket);
    }
    bool operator!=(const Self& other)
    {
        return !(*this == other);
    }
};
template <class Key, class Value, class KeyOfValue,
         class Hash = std::hash<Key>, 
         class KeyEqual = std::equal_to<Key>,
         class Alloc = alloc>
class HashTable
{
public:
    typedef Key                         key_type;
    typedef Value                       value_type;
    typedef std::size_t                 size_type;
    typedef std::ptrdiff_t              difference_type;
    typedef Hash                        hasher;
    typedef KeyEqual                    key_equal;
    typedef value_type&                 reference;
    typedef const value_type&           const_reference;
    typedef value_type*                 pointer;
    typedef const value_type*           const_pointer;
    
    typedef HashTableIterator<value_type>   iterator;
    typedef const iterator                  const_iterator;

    typedef tinystl::vector<tinystl::list<value_type>>   bucket_type;
    typedef typename tinystl::list<value_type>::iterator local_iterator;
    typedef const local_iterator                         const_local_iterator;

    typedef HashTable                   Self;

    enum { INIT_BUCKET_SIZE = 128, INIT_LOAD_FACTOR = 5 };
protected:
    bucket_type bucket_;
    size_type bucketSize_;
    size_type dataSize_;
    float factor_;
    hasher hash_;
    key_equal equal_;
    size_type entryIdx_;

public:
    explicit HashTable(size_type bucketSize = INIT_BUCKET_SIZE,
                       const hasher& hash = hasher(),
                       const key_equal& equal = key_equal())
        : bucket_(bucketSize),
          bucketSize_(bucketSize),
          dataSize_(0),
          factor_(INIT_LOAD_FACTOR),
          hash_(hash),
          equal_(equal),
          entryIdx_(bucketSize)
    {  }

    ~HashTable() {  }

    HashTable(const Self& other)
        : bucket_(other.bucket_),
          bucketSize_(other.bucketSize_),
          factor_(other.factor_),
          hash_(other.hash_),
          equal_(other.equal_),
          entryIdx_(other.entryIdx_)
    {  }

    HashTable(Self&& other)
        : bucket_(std::move(other.bucket_)),
          bucketSize_(std::move(other.bucketSize_)),
          factor_(std::move(other.factor_)),
          hash_(std::move(other.hash_)),
          equal_(std::move(other.equal_)),
          entryIdx_(std::move(other.entryIdx_))
    {  }

    Self& operator=(const Self& other)
    {
        Self tmp(other);
        this->swap(tmp);
        return *this;
    }

    Self& operator=(Self&& other)
    {
        Self tmp(std::move(other));
        this->swap(tmp);
        return *this;
    }

    iterator begin() noexcept { return iterator(entryIdx_, bucket_); }
    const_iterator begin() const noexcept { return const_iterator(entryIdx_, bucket_); }
    const_iterator cbegin() const noexcept { return const_iterator(entryIdx_, bucket_); }

    iterator end() noexcept { return iterator(bucketSize_, bucket_); }
    const_iterator end() const noexcept { return const_iterator(bucketSize_, bucket_); }
    const_iterator cend() const noexcept { return const_iterator(bucketSize_, bucket_); }

    bool empty() const noexcept { return  dataSize_ == 0; }
    size_type size() const noexcept { return dataSize_; }
    size_type max_size() const noexcept { return static_cast<size_type>(-1); }

    void clear() { bucket_.clear(); }
    
    iterator insert(const value_type& value);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    size_type erase(const key_type& key);

    iterator find(const key_type& key);
    const_iterator find(const key_type& key) const;

    size_type count(const key_type& key) const;
    
    tinystl::pair<iterator, iterator> equal_range(const key_type& key);

    void swap(Self& other)
    {
        tinystl::swap(bucket_, other.bucket_);
        tinystl::swap(bucketSize_, other.bucketSize_);
        tinystl::swap(factor_, other.factor_);
        tinystl::swap(entryIdx_, other.entryIdx_);
        tinystl::swap(dataSize_, other.dataSize_);
        tinystl::swap(hash_, other.hash_);
        tinystl::swap(equal_, other.equal_);
    }
public:
    local_iterator begin(size_type n) { return bucket_[n].begin(); }
    const_local_iterator begin(size_type n) const { return bucket_[n].begin(); }
    const_local_iterator cbegin(size_type n) const { return bucket_[n].begin(); }

    local_iterator end(size_type n) { return bucket_[n].end(); }
    const_local_iterator end(size_type n) const { return bucket_[n].end(); }
    const_local_iterator cend(size_type n) const { return bucket_[n].end(); }

    size_type bucket_count() const { return bucketSize_; }
    size_type max_bucket_count() const { return static_cast<size_type>(-1); }
    size_type bucket_size(size_type n) const { return bucket_[n].size(); }
    size_type bucket(const key_type& key) const { return hash_(key) % bucketSize_; }
    float load_factor() const { return size() / static_cast<float>(bucket_count()); }
    float max_load_factor() const { return factor_; }
    void max_load_factor(float ml)  { factor_ = ml; }

    void rehash(size_type count);
    void reserve(size_type count) { rehash(std::ceil(count / max_load_factor())); }

public:
    hasher hash_function() const { return hash_; }
    key_equal key_eq() const { return equal_; }

private:
    void updateEntryForInsert(size_type idx)
    {
        entryIdx_ = tinystl::min(entryIdx_, idx);
    }
    void updateEntryForErase(size_type idx)
    {
        if(idx == entryIdx_)
        {
            while(entryIdx_ != bucketSize_ && bucket_[entryIdx_].empty())
                ++entryIdx_;
        }
    }
    void rehashIfNeeded()
    {
        if(load_factor() >= max_load_factor())
            rehash(bucket_count() * 5);
    }

};
template <class Key, class Value, class KeyOfValue, class Hash, class KeyEqual, class Alloc>
typename HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::iterator
HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::insert(const value_type& value)
{
    iterator it = find(KeyOfValue()(value));
    if(it == end())
    {
        size_type idx = hash_(KeyOfValue()(value)) % bucketSize_;
        bucket_[idx].push_front(value);
        ++dataSize_;
        updateEntryForInsert(idx);
        rehashIfNeeded();
        return iterator(idx, bucket_, bucket_[idx].begin());
    }
    else
    {
        return it;
    }
}


template <class Key, class Value, class KeyOfValue, class Hash, class KeyEqual, class Alloc>
typename HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::iterator
HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::erase(const_iterator pos)
{
    size_type idx = pos.bucketIdx; 
    if(idx < size())
    {
        auto listIt = bucket_[idx].erase(pos.listIt);
        --dataSize_;
        updateEntryForErase(idx);
        if(listIt == bucket_[idx].end())
            return iterator(idx, bucket_, listIt)++;
        else
            return iterator(idx, bucket_, listIt);
    }
    return iterator(idx, bucket_, pos.listIt);
}


template <class Key, class Value, class KeyOfValue, class Hash, class KeyEqual, class Alloc>
typename HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::size_type
HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::erase(const key_type& key)
{
    size_type cnt = 0;
    size_type idx = hash_(key) % bucketSize_;
    bucket_[idx].remove_if([&cnt, &key](const value_type& value){
                                bool isSame = KeyEqual()(key, KeyOfValue()(value));
                                if(isSame)  ++cnt;
                                return isSame;
                           }); 
    dataSize_ -= cnt;
    updateEntryForErase(idx);
    return cnt;
}


template <class Key, class Value, class KeyOfValue, class Hash, class KeyEqual, class Alloc>
typename HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::iterator
HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::find(const key_type& key)
{
    size_type idx = hash_(key) % bucketSize_;
    auto it = tinystl::find_if(bucket_[idx].begin(), bucket_[idx].end(), [&key](const value_type& value){
                            return KeyEqual()(key, KeyOfValue()(value));
                        });
    if(it != bucket_[idx].end())
        return iterator(idx, bucket_, it);
    else
        return end();
}

template <class Key, class Value, class KeyOfValue, class Hash, class KeyEqual, class Alloc>
typename HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::const_iterator
HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::find(const key_type& key) const
{
    return const_cast<Self*>(this)->find(key);
}

template <class Key, class Value, class KeyOfValue, class Hash, class KeyEqual, class Alloc>
typename HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::size_type
HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::count(const key_type& key) const
{
    size_type idx = hash_(key) % bucketSize_;
    return tinystl::count_if(bucket_[idx].begin(), bucket_[idx].end(), 
                         [&key](const value_type& value){
                            return KeyEqual()(key, KeyOfValue()(value));
                         });
}


template <class Key, class Value, class KeyOfValue, class Hash, class KeyEqual, class Alloc>
tinystl::pair<typename HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::iterator,
              typename HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::iterator>
HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::equal_range(const key_type& key)
{
    iterator it = find(key);
    iterator first = it;
    iterator last = ++it;
    return tinystl::make_pair(first, last);
}



template <class Key, class Value, class KeyOfValue, class Hash, class KeyEqual, class Alloc>
void HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>::rehash(size_type count)
{
    bucket_type newBucket(count);
    for(iterator it = begin(); it != end(); ++it)
    {
        size_type idx = hash_(KeyOfValue()(*it)) % count;
        newBucket[idx].push_front(*it);
    }
    tinystl::swap(bucket_, newBucket);
    bucketSize_ = count;
}

template <class Key, class Value, class KeyOfValue, class Hash, class KeyEqual, class Alloc>
void swap(const HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>& lhs,
          const HashTable<Key, Value, KeyOfValue, Hash, KeyEqual, Alloc>& rhs)
{
    lhs.swap(rhs);
}


}
