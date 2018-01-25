#pragma once

#include "deque.h"

namespace tinystl
{

template <class T, class Container = deque<T>>
class stack
{
public:
    typedef Container                           container_type;
    typedef typename Container::value_type      value_type;
    typedef typename Container::size_type       size_type;
    typedef typename Container::reference       reference;
    typedef typename Container::const_reference const_reference;

protected:
    container_type cont_;
public:
    explicit stack(const Container& cont)
        : cont_(cont)
    {  }

    explicit stack(Container&& cont = Container())
        : cont_(cont)
    {  }

    stack(const stack& other)
        : cont_(other.cont_)
    {  }
    
    stack(stack&& other)
        : cont_(std::move(other.cont_))
    {  }

    stack& operator=(const stack& other) { cont_ = other.cont_; }

    stack& operator=(stack&& other) { cont_ = std::move(other.cont_); }

    reference top() { return cont_.front(); }
    const_reference top() const { return cont_.front(); }

    bool empty() const { return cont_.empty(); }
    size_type size() const { return cont_.size(); }

    void push(const value_type& value) { cont_.push_front(value); }
    void pop() { cont_.pop_front(); }

    void swap(stack& other) { cont_.swap(other.cont_); }
};


template <class T, class Container>
bool operator==(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs)
{
    stack<T, Container> lhsTmp = lhs;
    stack<T, Container> rhsTmp = rhs;
    while(!lhsTmp.empty() && !rhsTmp.empty())
    {
        if(lhsTmp.top() != rhsTmp.top())
            return false;
        lhsTmp.pop();
        rhsTmp.pop();
    }
    return lhsTmp.empty() && rhsTmp.empty();

}

template <class T, class Container>
bool operator!=(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs)
{
    return !(lhs == rhs);
}

}

