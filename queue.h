#pragma once

#include "deque.h"

namespace tinystl
{

template <class T, class Container = deque<T>>
class queue
{
public:
    typedef Container                           container_type;
    typedef typename Container::value_type      value_type;
    typedef typename Container::size_type       size_type;
    typedef typename Container::reference       reference;
    typedef typename Container::const_reference const_reference;

private:
    container_type cont_;

public:
    explicit queue(const container_type& cont)
        : cont_(cont)
    {  }
    
    explicit queue(container_type&& cont = container_type())
        : cont_(std::move(cont))
    {  }
    
    queue(const queue& other)
        : cont_(other.cont_)
    {  }

    queue(queue&& other)
        : cont_(std::move(other.cont_))
    {  }

    ~queue() {}

    queue& operator=(const queue& other)
    { cont_ = other.cont_; }

    queue& operator=(queue&& other)
    { cont_ = std::move(other.cont_); }

    reference front() { return cont_.front(); }
    const_reference front() const { return cont_.front(); }
    reference back()  { return cont_.back(); }
    const_reference back()  const { return cont_.back(); }

    bool empty() const { return cont_.empty(); }
    size_type size() const { return cont_.size(); }

    void push(const value_type& value) { cont_.push_back(value); }
    void pop() { cont_.pop_front(); }

    void swap(queue& other) noexcept { cont_.swap(other.cont_); }

};


template <class T, class Container>
bool operator==(const queue<T, Container>& lhs,
                const queue<T, Container>& rhs)
{
    auto ltmp(lhs);
    auto rtmp(rhs);
    while(!ltmp.empty() && !rtmp.empty())
    {
        if(ltmp.front() != rtmp.front())
            return false;
        ltmp.pop();
        rtmp.pop();
    }
    return ltmp.empty() && rtmp.empty();
}

template <class T, class Container>
bool operator!=(const queue<T, Container>& lhs,
                const queue<T, Container>& rhs)
{
    return !(lhs == rhs);  
}
}
