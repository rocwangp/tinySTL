#pragma once

#include "iterator.h"
#include "alloc.h"
#include "construct.h"

#include <iostream>

namespace tinystl
{

template <class T>
struct ListNode
{
    ListNode(const T& v, ListNode<T>* p, ListNode* n)
        : val(v),
          prev(p),
          next(n)
    {  }
    T val;
    ListNode<T>* prev;
    ListNode<T>* next;
};

template <class U>
class ListIterator : 
    public iterator<bidirectional_iterator_tag, U>
{
public:
   ListNode<U>* node_; 
public:
    typedef ListIterator<U> Self;
    typedef U       value_type;
    ListIterator()
        : node_(nullptr)
    { }
    ListIterator(ListNode<U>* header)
        : node_(header)
    { }
    ListIterator(const Self& it)
        : node_(it.node_)
    { }

    Self operator++(int)
    {
        Self old = *this;
        ++(*this);
        return old;
    }
    Self& operator++()
    {
       node_ = node_->next;
       return *this;
    }
    Self operator--(int)
    {
        Self old = *this;
        --(*this);
        return old;
    }
    Self& operator--()
    {
        node_ = node_->prev;
        return *this;
    }
    
    bool operator==(const Self& it) const
    {
        return node_ == it.node_;
    }
    bool operator!=(const Self& it) const
    {
        return node_ != it.node_;
    }

    Self& operator=(const Self& it)
    {
        node_ = it.node_;
        return *this;
    }

    U& operator*() { return node_->val; }
    U* operator->() { return &node_->val; }

};


template <class T, class Alloc = alloc>
class list
{

public:
    typedef T                       value_type;
    typedef T*                      pointer;
    typedef const T*                const_pointer;
    typedef T&                      reference;
    typedef const T&                const_reference;
    typedef std::ptrdiff_t          difference_type;
    typedef std::size_t             size_type;
    typedef ListIterator<T>         iterator;
    typedef const ListIterator<T>   const_iterator;
    typedef ListNode<T>*            node_pointer;
private:
    /* 头尾节点，额外的两个指针 */
    iterator header_;
    iterator tail_;
    typedef  SimpleAlloc<ListNode<T>, Alloc> nodeAllocator;
public:
    /* 构造，析构，赋值 */
    list(); 
    explicit list(size_type count, const value_type& value = value_type());
    list(const list& other);
    list(list&& other);
    list(std::initializer_list<T> init);

    template <class InputIterator>
        list(InputIterator front, InputIterator back);
    ~list();

    list& operator=(const list& l);
    list& operator=(list&& l);
    list& operator=(std::initializer_list<T> ilist);

    void assign(size_type count, const value_type& value);
    template <class InputIterator>
        void assign(InputIterator first, InputIterator last);
    void assign(std::initializer_list<T> ilist);

    /* Element access */
    reference front() { return *begin(); } 
    const_reference front() const { return *begin(); }
    reference back()  { return *(--end()); }
    const_reference back() const  { return const_reference(*(--end())); }


    /* Iterators */
    iterator begin() noexcept { return iterator(header_.node_->next); }
    const_iterator begin() const noexcept { return iterator(header_.node_->next); }
    iterator end() noexcept { return tail_; }
    const_iterator end() const noexcept { return tail_; }


    /* Capacity */
    bool empty() const noexcept { return begin() == end(); }
    size_type size() const noexcept ;
    size_type max_size() const noexcept { return size(); }

    /* Modifiers */
    void clear() noexcept { erase(begin(), end()); }
    iterator insert(const_iterator pos, const value_type& value);
    iterator insert(const_iterator pos, size_type count, const value_type& value);
    iterator insert(const_iterator pos, std::initializer_list<T> ilist);
    template <class InputIterator>
        iterator insert(const_iterator, InputIterator first, InputIterator last);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    void push_back(const value_type& val) ;
    void push_front(const value_type& val);
    void pop_back();
    void pop_front();

    void resize(size_type count) { resize(count, value_type()); }
    void resize(size_type count, const value_type& value);

    void swap(list& other);

    /* Operations */
    void splice(const_iterator pos, list& other);
    void splice(const_iterator pos, list& other, const_iterator it); 
    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);

    void merge(list& other);
    template <class Compare>
        void merge(list& other, Compare compare);

    void remove(const value_type& val);
    template <class UnaryPredicate>
        void remove_if(UnaryPredicate p);

    void reverse() noexcept;
    void unique();
    template <class BinaryPredicate>
        void unique(BinaryPredicate p);

    void sort();
    template <class Compare>
        void sort(Compare compare);

private:
    void ctorAux(size_type count, const value_type& value, std::true_type);
    template <class InputIterator>
        void ctorAux(InputIterator first, InputIterator last, std::false_type);

    void assignAux(size_type count, const value_type& valu, std::true_type);
    template <class InputIterator>
        void assignAux(InputIterator first, InputIterator last, std::false_type);

    iterator insertAux(const_iterator pos, size_type count, const value_type& value, std::true_type);
    template <class InputIterator>
        iterator insertAux(const_iterator pos, InputIterator first, InputIterator last, std::false_type);
private:
   node_pointer create_node(const value_type& val); 
};


template <class T, class Alloc>
typename list<T, Alloc>::node_pointer
list<T, Alloc>::create_node(const value_type& val)
{
    node_pointer ptr = nodeAllocator::allocate(1);
    construct(ptr, ListNode<T>(val, nullptr, nullptr));
    return ptr;
}


/* **********************************构造，析构，拷贝，赋值*********************************** */
template <class T, class Alloc>
void list<T, Alloc>::ctorAux(size_type count, const value_type& value, std::true_type)
{
    while(count--)
        push_back(value);

}

/* std::is_integral<InputIterator>()为std::false_type时调用
 * 解决InputIterator被生成int类型 */
template <class T, class Alloc>
template <class InputIterator>
void list<T, Alloc>::ctorAux(InputIterator first, InputIterator last, std::false_type)
{
    while(first != last)
        push_back(*first++);
}


template <class T, class Alloc>
list<T, Alloc>::list()
{
    header_.node_ = create_node(value_type());
    tail_.node_ = create_node(value_type());
    header_.node_->next = tail_.node_;
    tail_.node_->prev = header_.node_;
}

template <class T, class Alloc>
list<T, Alloc>::list(size_type count, const value_type& value)
    : list()
{
    ctorAux(count, value, std::is_integral<size_type>());
}

template <class T, class Alloc>
template <class InputIterator>
list<T, Alloc>::list(InputIterator front, InputIterator back)
    : list()
{
    /* 使用std::is_integral主要是解决InputIterator被生成int类型
     * 如调用list<int> l(10, 0); */
    ctorAux(front, back, std::is_integral<InputIterator>());
}

template <class T, class Alloc>
list<T, Alloc>::list(const list& other)
    : list(other.begin(), other.end())
{
  
}

/* 移动构造函数，使用other的所有内存 */
template <class T, class Alloc>
list<T, Alloc>::list(list&& other)
    : list()
{
    if(!other.empty())
    {
        header_.node_->next = std::move(other.header_.node_->next);
        tail_.node_->prev = std::move(other.tail_.node_->prev);
    }
    /* 改变other的成员防止other析构时释放数据 */
    other.header_.node_->next = other.tail_.node_;
    other.tail_.node_->prev = other.header_.node_;
}


/* 列表初始化构造函数 
 * 如list<int> l{1, 2, 3, 4, 5};*/ 
template <class T, class Alloc>
list<T, Alloc>::list(std::initializer_list<T> init)
    : list(init.begin(), init.end())
{
    
}

template <class T, class Alloc>
list<T, Alloc>& 
list<T, Alloc>::operator=(const list<T, Alloc>& l)
{
    if(&l != this)
    {
        list<T, Alloc> tmp(l);
        std::swap(tmp, *this);
    }
    return *this;
}


template <class T, class Alloc>
list<T, Alloc>&
list<T, Alloc>::operator=(list<T, Alloc>&& l)
{
    if(&l != this)
    {
        list<T, Alloc> tmp(std::move(l));
        std::swap(tmp, *this);
    }
    return *this;
}

template <class T, class Alloc>
list<T, Alloc>&
list<T, Alloc>::operator=(std::initializer_list<T> ilist)
{
    list tmp(ilist);
    std::swap(tmp, *this);
}

template <class T, class Alloc>
list<T, Alloc>::~list<T, Alloc>()
{
    erase(begin(), end());
    destroy(header_.node_);
    destroy(tail_.node_);
    nodeAllocator::deallocate(this->header_.node_);
    nodeAllocator::deallocate(this->tail_.node_);
}


template <class T, class Alloc>
void list<T, Alloc>::assignAux(size_type count, const value_type& value, std::true_type)
{
    list tmp(count, value);
    std::swap(tmp, *this);
}

template <class T, class Alloc>
template <class InputIterator>
void list<T, Alloc>::assignAux(InputIterator first, InputIterator last, std::false_type)
{
    list tmp(first, last);
    std::swap(tmp, *this);
}

template <class T, class Alloc>
void list<T, Alloc>::assign(size_type count, const value_type& value)
{
    assign(count, value, std::is_integral<value_type>());
}


template <class T, class Alloc>
template <class InputIterator>
void list<T, Alloc>::assign(InputIterator first, InputIterator last)
{
    assign(first, last, std::is_integral<InputIterator>());
}

template <class T, class Alloc>
void list<T, Alloc>::assign(std::initializer_list<T> ilist)
{
    list tmp(ilist);
    std::swap(tmp, *this);
}

/* ****************************大小，容量相关************************ */

/* FIXME: 使用std::distance(begin(), end())会报错 */
template <class T, class Alloc>
typename list<T, Alloc>::size_type
list<T, Alloc>::size() const noexcept
{
    iterator it = begin();
    size_type n = 0;
    while(it != end())
    {
        ++n;
        ++it;
    }
    return n;
}


/* *************************************修改相关**************************************** */


/* *********************************插入********************************* */
template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::insertAux(const_iterator pos, size_type count, const value_type& value, std::true_type)
{
   node_pointer prevNode = pos.node_->prev;
   node_pointer resultNode = prevNode;
   while(count--)
   {
       node_pointer newNode = create_node(value);
       prevNode->next = newNode;
       newNode->prev = prevNode;
       prevNode = newNode;
   }
   prevNode->next = pos.node_;
   pos.node_->prev = prevNode;
   return iterator(resultNode->next);
}

template <class T, class Alloc>
template <class InputIterator>
typename list<T, Alloc>::iterator
list<T, Alloc>::insertAux(const_iterator pos, InputIterator first, InputIterator last, std::false_type)
{
    node_pointer prevNode = pos.node_->prev;
    node_pointer resultNode = prevNode;
    while(first != last)
    {
        node_pointer newNode = create_node(*first);
        prevNode->next = newNode;
        newNode->prev = prevNode;
        prevNode = newNode;
        ++first;
    }
    prevNode->next = pos.node_;
    pos.node_->prev = prevNode;
    return iterator(resultNode->next);
}


template <class T, class Alloc>
typename list<T, Alloc>::iterator 
list<T, Alloc>::insert(const_iterator pos, const value_type& value)
{
    return insert(pos, 1, value);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator 
list<T, Alloc>::insert(iterator pos, size_type count, const value_type& value)
{
    return insertAux(pos, count, value, std::is_integral<size_type>());
}

template <class T, class Alloc>
template <class InputIterator>
typename list<T, Alloc>::iterator
list<T, Alloc>::insert(const_iterator pos, InputIterator first, InputIterator last)
{
    return insertAux(pos, first, last, std::is_integral<InputIterator>());
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::insert(const_iterator pos, std::initializer_list<T> ilist)
{
    return insert(pos, ilist.begin(), ilist.end());
}


/* *********************************删除********************************* */
template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::erase(const_iterator pos)
{
    iterator last = pos;
    iterator first = last++;
    return erase(first, last);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::erase(const_iterator first, const_iterator last)
{
    node_pointer resultNode = last.node_;
    first.node_->prev->next = last.node_;
    last.node_->prev = first.node_->prev;
    for(iterator it = first; it != last; ++it)
    {
        destroy(it.node_);
        nodeAllocator::deallocate(it.node_);
    }
    return iterator(resultNode);
}


/* *********************************追加********************************* */
template <class T, class Alloc>
void list<T, Alloc>::push_back(const value_type& value)
{
    insert(end(), value);
} 

template <class T, class Alloc>
void list<T, Alloc>::push_front(const value_type& value)
{
    insert(begin(), value);
}

/* *********************************弹出********************************* */
template <class T, class Alloc>
void list<T, Alloc>::pop_back()
{
    erase(--end());
}

template <class T, class Alloc>
void list<T, Alloc>::pop_front()
{
    erase(begin());
}

/* *******************************重置大小******************************* */
template <class T, class Alloc>
void list<T, Alloc>::resize(size_type count, const value_type& value)
{
    int n = static_cast<int>(count) - static_cast<int>(size());
    if(n > 0)
    {
        while(n--)
            push_back(value);
    }
    else
    {
        while(n++)
            pop_back();
    }
}

/* *********************************交换********************************* */
template <class T, class Alloc>
void list<T, Alloc>::swap(list& other)
{
    std::swap(header_, other.header_);
    std::swap(tail_, other.tail_);
}


/* *********************************合并********************************* */
template <class T, class Alloc>
void list<T, Alloc>::merge(list& l)
{
    merge(l, std::less<value_type>());
}


template <class T, class Alloc>
template <class Compare>
void list<T, Alloc>::merge(list& other, Compare compare)
{
    iterator it = header_;
    iterator lhs = begin();
    iterator rhs = other.begin();
    while(lhs != end() && rhs != other.end())
    {
        if(compare(*lhs, *rhs))
        {
            it.node_->next = lhs.node_;
            lhs.node_->prev = it.node_;
            ++lhs;
        }
        else
        {
            it.node_->next = rhs.node_;
            rhs.node_->prev = it.node_;
            ++rhs;
        }
        ++it;
    }
    if(lhs != end())
    {
        it.node_->next = lhs.node_;
        lhs.node_->prev = it.node_;
    }
    else
    {
        it.node_->next = rhs.node_;
        rhs.node_->prev = it.node_;
        tail_.node_->prev = other.tail_.node_->prev;
        tail_.node_->prev->next = tail_.node_;
    }
    other.header_.node_->next = other.tail_.node_;
    other.tail_.node_->prev = other.header_.node_;
}

/* *********************************切分********************************* */
template <class T, class Alloc>
void list<T, Alloc>::splice(const_iterator pos, list& other)
{
    splice(pos, other, other.begin(), other.end());
}

template <class T, class Alloc>
void list<T, Alloc>::splice(const_iterator pos, list& other, const_iterator it)
{
    iterator first = it;
    iterator last = it;
    splice(pos, other, first, ++last);
}

/* 切分链表，将other链表的[first, last)移动到pos上，other中的[first, last)被删除 */
template <class T, class Alloc>
void list<T, Alloc>::splice(const_iterator pos, list& other, const_iterator first, const_iterator last)
{
    if(&other == this)
        return;
    node_pointer prevNode = pos.node_->prev;
    node_pointer lastNode = last.node_->prev;
    first.node_->prev->next = last.node_;
    last.node_->prev = first.node_->prev;
    prevNode->next = first.node_;
    first.node_->prev = prevNode;
    lastNode->next = pos.node_;
    pos.node_->prev = lastNode;
}


/* *********************************移除********************************* */
template <class T, class Alloc>
void list<T, Alloc>::remove(const value_type& val)
{
    remove_if([&val](const value_type& value) { return value == val; });
}


template <class T, class Alloc>
template <class UnaryPredicate>
void list<T, Alloc>::remove_if(UnaryPredicate p)
{
    iterator first = begin();
    iterator last = end();
    while(first != last)
    {
        if(p(*first))
            erase(first++);
        else
            ++first;
    }
}


/* ********************************逆序********************************* */
template <class T, class Alloc>
void list<T, Alloc>::reverse() noexcept
{
    node_pointer prevNode = header_.node_;
    node_pointer curNode = prevNode->next;
    prevNode->next = nullptr;
    while(curNode)
    {
       node_pointer nextNode = curNode->next;
       curNode->next = prevNode;
       curNode->prev = nullptr;
       prevNode->prev = curNode;
       prevNode = curNode;
       curNode = nextNode;
    }
    std::swap(header_, tail_);
}


/* ********************************去重********************************* */
template <class T, class Alloc>
void list<T, Alloc>::unique()
{
    unique([](const value_type& lhs, const value_type& rhs) { return lhs == rhs; });
}

template <class T, class Alloc>
template <class BinaryPredicate>
void list<T, Alloc>::unique(BinaryPredicate p)
{
    iterator first = begin();
    iterator last = end();
    iterator next = first;
    while(++next != last)
    {
        if(p(*first, *next))
            erase(next);
        else
            first = next;
        next = first;
    }
}



/* ********************************排序********************************* */
template <class T, class Alloc>
void list<T, Alloc>::sort()
{
    sort([](const value_type& lhs, const value_type& rhs) { return lhs < rhs; });
}

/* 链表的归并排序 */
template <class T, class Alloc>
template <class Compare>
void list<T, Alloc>::sort(Compare compare)
{
    if(size() <= 1) return;
    iterator it = begin();
    size_type n = size() / 2;
    while(n--)  ++it;
    /* 将链表切分成两半 */
    list other;
    other.splice(other.begin(), *this, begin(), it);
    /* 每一半分别排序 */
    sort(compare);
    other.sort(compare);
    /* 合并 */
    merge(other, compare);
}



/* ********************************************运算相关***************************************** */
template <class T, class Alloc>
bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
{
    if(rhs.size() != rhs.size())  return false;
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    while(lit != lhs.end())
    {
        if(*lit != *rit)
            return false;
        ++lit;
        ++rit;
    }
    return true;
}


template <class T, class Alloc>
bool operator!=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
{
    return !(lhs == rhs);
}

/* template <class T, class Alloc> */
/* bool operator<(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) */
/* { */
/*     return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); */
/* } */

/* template <class T, class Alloc> */
/* bool operator>(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) */
/* { */
/*     return std::lexicographical_compare(lhs.begin(), lhs.end(), */ 
/*                                         rhs.begin(), rhs.end(), */ 
/*                                         [](const T& n1, const T& n2){ */
/*                                             return n1 > n2; */
/*                                         }); */
/* } */


/* template <class T, class Alloc> */
/* bool operator<=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) */
/* { */
/*     return !(lhs > rhs); */
/* } */

/* template <class T, class Alloc> */
/* bool operator>=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) */
/* { */
/*     return !(lhs < rhs); */
/* } */



}
