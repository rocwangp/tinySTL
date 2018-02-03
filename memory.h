#pragma once

#include <functional>
#include <atomic>
#include <iostream>

#include "algorithm.h"
#include "alloc.h"


namespace tinystl
{



template <class T>
class default_delete
{
public:
    constexpr default_delete() noexcept
        : deleter_([](T* ptr) { delete ptr; })
    {  }

    template <class U>
    default_delete(const default_delete<U>& d) noexcept
        : deleter_(d)
    {  }

    void operator()(T* ptr) const
    {
        deleter_(ptr);
    }
protected:
    std::function<void(T*)> deleter_;
};


template <class T>
class default_delete<T[]>
{
    constexpr default_delete() noexcept
        : deleter_([](T* ptr) { delete[] ptr; })
    {  }

    template <class U>
    default_delete(const default_delete<U[]>& d) noexcept
        : deleter_(d)
    {  }

    template <class U>
    void operator()(U ptr) const
    {
        deleter_(ptr);
    }
protected:
    std::function<void(T[])> deleter_;
};

template <class T>
class shared_ptr
{
public:
    typedef T   element_type;

protected:
    class ControlBlock;
    ControlBlock* ctolBlkPtr_;
    T* dataPtr_;
public:

    constexpr shared_ptr() noexcept
        : shared_ptr(nullptr)
    {
    }

    constexpr shared_ptr(std::nullptr_t) noexcept
    {
        ctolBlkPtr_ = new ControlBlock();
        dataPtr_ = nullptr;
    }

    template <class Y>
    explicit shared_ptr(Y* ptr)
    {
        initCtolBlock(ptr, default_delete<T>());
        dataPtr_ = ptr;
    }

    template <class Y, class Deleter>
    shared_ptr(Y* ptr, Deleter d)
    {
        initCtolBlock(ptr, d);
        dataPtr_ = ptr;
    }

    shared_ptr(const shared_ptr& other) noexcept
        : ctolBlkPtr_(other.ctolBlkPtr_),
          dataPtr_(other.dataPtr_)
    {
        ctolBlkPtr_->incRefCount();
    }

    template <class Y>
    shared_ptr(const shared_ptr<Y>& other) noexcept
        : ctolBlkPtr_(other.ctolBlkPtr_),
          dataPtr_(other.dataPtr_)
    {
        ctolBlkPtr_->incRefCount();
    }

    template <class Y>
    shared_ptr(shared_ptr<Y>&& other) noexcept
        : ctolBlkPtr_(std::move(other.ctolBlkPtr_)),
          dataPtr_(std::move(other.dataPtr_))
    {
        other.ctolBlkPtr_ = nullptr;
        other.dataPtr_ = nullptr;
    }


    ~shared_ptr()
    {
        if(ctolBlkPtr_)
        {
            ctolBlkPtr_->decRefCount();
            if(!ctolBlkPtr_->refCount && !ctolBlkPtr_->weakCount)
                delete ctolBlkPtr_;
        }
    }
    
    template <class Y>
    shared_ptr<Y>& operator=(const shared_ptr<Y>& other)
    {
        ctolBlkPtr_->decRefCount();
        ctolBlkPtr_ = other.ctolBlkPtr_;
        dataPtr_ = other.dataPtr_;
        ctolBlkPtr_->incRefCount();
        return *this;
    }

    template <class Y>
    shared_ptr<Y>& operator=(shared_ptr<Y>&& other)
    {
        ctolBlkPtr_->decRefCount();
        ctolBlkPtr_ = std::move(other.ctolBlkPtr_);
        dataPtr_ = std::move(other.dataPtr_);
        other.ctolBlkPtr_ = nullptr;
        other.dataPtr_ = nullptr;
        return *this;
    }


    void reset() noexcept
    {
        shared_ptr().swap(*this);
    }

    template <class Y>
    void reset(Y *ptr)
    {
        shared_ptr(ptr).swap(*this);
    }

    template <class Y, class Deleter>
    void reset(Y* ptr, Deleter d)
    {
        shared_ptr(ptr, d).swap(*this);
    }

    T* get() const noexcept 
    {
        return dataPtr_;
    }

    T& operator*() const noexcept
    {
        return *dataPtr_;
    }

    T* operator->() const noexcept
    {
        return dataPtr_;
    }

    long use_count() const noexcept
    {
        return ctolBlkPtr_->refCount;
    }

    explicit operator bool() const noexcept
    {
        return dataPtr_ != nullptr;
    }


public:
    void swap(shared_ptr& other) noexcept
    {
        tinystl::swap(ctolBlkPtr_, other.ctolBlkPtr_);
        tinystl::swap(dataPtr_, other.dataPtr_);
    }
private:
    template <class Y, class Deleter>
    void initCtolBlock(Y* ptr, Deleter d)
    {
        ctolBlkPtr_ = new ControlBlock(ptr, d);
    }
  
};

template <class T>
class shared_ptr<T>::ControlBlock
{
public:

    typedef SimpleAlloc<ControlBlock, alloc> ctolBlkAllocator;
public:
    T* dataPtr;
    std::atomic<long> refCount;
    std::atomic<int> weakCount;
    std::function<void(T* p)> deleter;

public:
    constexpr ControlBlock() noexcept
        : dataPtr(nullptr),
          refCount(1),
          weakCount(0),
          deleter(tinystl::default_delete<T>())
    {  }

    template <class Y, class Deleter>
    explicit ControlBlock(Y* ptr, Deleter d = tinystl::default_delete<T>())
        : dataPtr(ptr),
          refCount(1),
          weakCount(0),
          deleter(d)
    {  }
    
 


public:
    void decRefCount()
    {
        --refCount;
        tryDeleteObjManaged();
    }

    void incRefCount()
    {
        ++refCount;
    }

    void incWeakCount()
    {
        ++weakCount;
    }

    void decWeakCount()
    {
        --weakCount;
    }
private:
    void tryDeleteObjManaged()
    {
        if(!refCount)
        {
            deleter(dataPtr);
        }
    }
};


template <class T, class... Args>
shared_ptr<T> make_shared(Args&&... args)
{
    auto ptr = new T(std::forward<Args>(args)...);
    return shared_ptr<T>(ptr);
}

template <class T, class U>
bool operator==(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) noexcept
{
    return lhs.get() == rhs.get();
}



template <class T>
bool operator==(const shared_ptr<T>& lhs, std::nullptr_t rhs) noexcept
{
    return lhs.get() == rhs;
}
template <class T>
bool operator==(std::nullptr_t lhs, const shared_ptr<T>& rhs) noexcept
{
    return lhs == rhs.get();
}

template <class T, class U>
bool operator!=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) noexcept
{
    return !(lhs == rhs);
}


template <class T>
bool operator!=(const shared_ptr<T>& lhs, std::nullptr_t rhs) noexcept
{
    return lhs.get() != rhs;
}
template <class T>
bool operator!=(std::nullptr_t lhs, const shared_ptr<T>& rhs) noexcept
{
    return lhs != rhs.get();
}




template <class T, class Deleter = tinystl::default_delete<T>>
class unique_ptr
{
public:
    typedef T*          pointer;
    typedef T           element_type;
    typedef Deleter     deleter_type;

protected:
    pointer ptr_;
    deleter_type deleter_;

public:
    constexpr unique_ptr() noexcept
        : ptr_(nullptr)
    {  }

    explicit unique_ptr(pointer ptr) noexcept
        : ptr_(ptr)
    {  }

    unique_ptr(unique_ptr&& u) noexcept
        : ptr_(std::move(u.ptr_)),
          deleter_(std::move(u.deleter_))
    { u.ptr_ = nullptr; }

    ~unique_ptr() { get_deleter()(get()); }

    unique_ptr& operator=(unique_ptr&) = delete;
    unique_ptr& operator=(unique_ptr&& other)
    {
        ptr_ = std::move(other.ptr_);
        deleter_ = std::move(other.deleter_);
        other.ptr_ = nullptr;
    }

public:
    pointer release() noexcept
    {
        pointer tmp = ptr_;
        ptr_ = nullptr;
        return tmp;
    }

    void reset(pointer ptr = pointer()) noexcept
    {
        pointer tmp = ptr_;
        ptr_ = ptr;
        get_deleter()(tmp);
    }

    void swap(unique_ptr& other) noexcept
    {
        std::swap(ptr_, other.ptr_);
        std::swap(deleter_, other.deleter_);
    }

    typename std::add_lvalue_reference<T>::type operator*() const 
    {
        return *ptr_;
    }

    pointer operator->() const noexcept
    {
        return ptr_;
    }

public:
    pointer get() const noexcept { return ptr_; }

    Deleter& get_deleter() noexcept { return deleter_; }
    const Deleter& get_deleter() const noexcept { return deleter_; }
    explicit operator bool() const noexcept { return get() != nullptr; }

};

template <class T, class ...Args>
unique_ptr<T> make_unique(Args&&... args)
{
    auto ptr = new T(std::forward<Args>(args)...);
    return unique_ptr<T>(ptr);
}


template <class T, class Deleter>
void swap(unique_ptr<T, Deleter>& lhs,
          unique_ptr<T, Deleter>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <class T1, class D1, class T2, class D2>
bool operator==(const unique_ptr<T1, D1>& x, 
                const unique_ptr<T2, D2>& y)
{
    return x.get() == y.get();
}

template <class T, class D>
bool operator==(const unique_ptr<T, D>& x, 
                std::nullptr_t) noexcept
{
    return x.get() == nullptr;
}

template <class T1, class D1, class T2, class D2>
bool operator==(std::nullptr_t, 
                const unique_ptr<T2, D2>& y)
{
    return nullptr == y.get();
}

template <class T1, class D1, class T2, class D2>
bool operator!=(const unique_ptr<T1, D1>& x, 
                const unique_ptr<T2, D2>& y)
{
    return !(x == y);
}

template <class T, class D>
bool operator!=(const unique_ptr<T, D>& x, 
                std::nullptr_t) noexcept
{
    return !(x == nullptr);
}

template <class T1, class D1, class T2, class D2>
bool operator!=(std::nullptr_t, 
                const unique_ptr<T2, D2>& y)
{
    return !(nullptr == y);
}


template <class InputIt, class ForwardIt>
ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first)
{
    while(first != last)
    {
        tinystl::construct(d_first, *first);
        ++first;
        ++d_first;
    }
    return d_first;
}

template <class InputIt, class Size, class ForwardIt>
ForwardIt uninitialized_copy_n(InputIt first, Size count, ForwardIt d_first)
{
    while(count--)
    {
        tinystl::construct(d_first, *first);
        ++first;
        ++d_first;
    }
    return d_first;
}


template <class ForwardIt, class T>
ForwardIt uninitialized_fill(ForwardIt first, ForwardIt last, const T& value)
{
    while(first != last)
    {
        tinystl::construct(first, value);
        ++first;
    }
    return first;
}


template <class ForwardIt, class Size, class T>
ForwardIt uninitialized_fill_n(ForwardIt first, Size count, const T& value)
{
    while(count--)
    {
        tinystl::construct(first, value);
        ++first;
    }
    return first;
}
}
