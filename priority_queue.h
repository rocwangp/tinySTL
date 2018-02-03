#pragma once

#include "vector.h"
#include "algorithm.h"

namespace tinystl
{

template <class T, 
         class Container = vector<T>, 
         class Compare = std::less<typename Container::value_type>>
class priority_queue
{
public:
    typedef Container                           container_type;
    typedef Compare                             value_compare;
    typedef typename Container::value_type      value_type;
    typedef typename Container::size_type       size_type;
    typedef typename Container::reference       reference;
    typedef typename Container::const_reference const_reference;

protected:
    container_type cont_;
    value_compare comp_;

public:
    priority_queue(const value_compare& compare, const container_type& cont)
        : cont_(cont),
          comp_(compare)
    { tinystl::make_heap(cont_.begin(), cont_.end(), compare); }

    explicit priority_queue(const value_compare& compare = value_compare(),
                            container_type&& cont = container_type())
        : cont_(std::move(cont)),
          comp_(compare)
    { tinystl::make_heap(cont_.begin(), cont_.end(), compare); }

    priority_queue(const priority_queue& other)
        : cont_(other.cont_),
          comp_(other.comp_)
    {  }

    priority_queue(priority_queue&& other)
        : cont_(std::move(other.cont_)),
          comp_(std::move(other.comp_))
    { }

    template <class InputIterator>
    priority_queue(InputIterator first, 
                   InputIterator last,
                   const value_compare& compare,
                   const container_type& cont)
        : cont_(cont),
          comp_(compare)
    {
        cont_.insert(cont_.end(), first, last);
        tinystl::make_heap(cont_.begin(), cont_.end(), compare);
    }

    template <class InputIterator>
    priority_queue(InputIterator first,
                   InputIterator last,
                   const value_compare& compare = value_compare(),
                   container_type&& cont = container_type())
        : cont_(std::move(cont)),
          comp_(compare)
    {
        cont_.insert(cont_.end(), first, last);
        tinystl::make_heap(cont_.begin(), cont_.end(), compare);
    
    }

    ~priority_queue() {}

    priority_queue& operator=(const priority_queue& other)
    { tinystl::swap(priority_queue(other), *this); }

    priority_queue& operator=(priority_queue&& other)
    { tinystl::swap(priority_queue(std::move(other)), *this); }

    const_reference top() const { return cont_.front(); }

    bool empty() const { return cont_.empty(); }
    size_type size() const { return cont_.size(); }

    void push(const value_type& value)
    {
        cont_.insert(cont_.end(), value);
        tinystl::push_heap(cont_.begin(), cont_.end(), comp_);
    }

    void pop()
    {
        tinystl::pop_heap(cont_.begin(), cont_.end(), comp_);
        cont_.pop_back();
    }

    void swap(priority_queue& other) noexcept { cont_.swap(other.cont_); }

    void printContainer()
    {
        for(auto it = cont_.begin(); it != cont_.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;
    }
};




}
