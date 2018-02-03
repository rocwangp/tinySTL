#pragma once

#include <cstdint>


namespace tinystl {






struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

/* 迭代器基类，所有容易内部实现的迭代器都需要继承这个类
 * 目的是让所有的迭代器都需要定义下面这5个内嵌类型 */
template <class Category, 
         class T, 
         class Distance = std::ptrdiff_t, 
         class Pointer = T*, 
         class Reference = T&>
struct iterator{
    typedef Category    iterator_category;
    typedef T           value_type;
    typedef Distance    difference_type;
    typedef Pointer     pointer;
    typedef Reference   reference;
};


/* traits技法，迭代器类型萃取器
 * 通过iterator_traits<MyIter>::value_type等就可以获取MyIter迭代器内部的类型 */
template <class Iterator>
struct iterator_traits{
    typedef typename Iterator::iterator_category    iterator_category;
    typedef typename Iterator::value_type           value_type;
    typedef typename Iterator::difference_type      difference_type;
    typedef typename Iterator::pointer         pointer;
    typedef typename Iterator::reference       reference;
};


/* 由于原生指针T*没有内嵌类型，但是也可以看成是迭代器的一种
 * 所以为原生指针T*偏特化一个iterator_traits */
template <class T>
struct iterator_traits<T*>{
    typedef random_access_iterator_tag      iterator_category;
    typedef T                               value_type;
    typedef std::ptrdiff_t                  difference_type;
    typedef T*                              pointer;
    typedef T&                              reference;
};

/* 对const T*的偏特化 */
template <class T>
struct iterator_traits<const T*>{
    typedef random_access_iterator_tag      iterator_category;
    typedef T                               value_type;
    typedef std::ptrdiff_t                  difference_type;
    typedef T*                              pointer;
    typedef T&                              reference;
};

/* *********************************************************** *
 *                   五种迭代器的定义                          *
 *               只有iterator_category不同                     *
 * *********************************************************** */
template <class T, class Distance>
struct input_iterator{
    typedef input_iterator_tag              iterator_category;
    typedef T                               value_type;
    typedef Distance                        difference_type;
    typedef T*                              pointer;
    typedef T&                              reference;
};

template <class T, class Distance>
struct output_iterator{
    typedef output_iterator_tag             iterator_category;
    typedef T                               value_type;
    typedef Distance                        difference_type;
    typedef T*                              pointer;
    typedef T&                              reference;
};


template <class T, class Distance>
struct forward_iterator{
    typedef forward_iterator_tag             iterator_category;
    typedef T                               value_type;
    typedef Distance                        difference_type;
    typedef T*                              pointer;
    typedef T&                              reference;
};


template <class T, class Distance>
struct bidirectional_iterator{
    typedef bidirectional_iterator_tag      iterator_category;
    typedef T                               value_type;
    typedef Distance                        difference_type;
    typedef T*                              pointer;
    typedef T&                              reference;
};

template <class T, class Distance>
struct random_access_iterator{
    typedef random_access_iterator_tag      iterator_category;
    typedef T                               value_type;
    typedef Distance                        difference_type;
    typedef T*                              pointer;
    typedef T&                              reference;
};


/* 用于返回iterator_category对象，即上述五个*_iterator_tag对象 */
template<class InputIterator>
inline typename tinystl::iterator_traits<InputIterator>::iterator_category
iterator_category(InputIterator) {
    typedef typename tinystl::iterator_traits<InputIterator>::iterator_category category;
    return category();
}

/* 用于返回differnce_type类型的初值0 */
template <class Iterator>
inline typename tinystl::iterator_traits<Iterator>::difference_type
difference_type(Iterator) {
    return static_cast<typename iterator_traits<Iterator>::difference_type>(0);
}


/* *************************************************************** *
 *                           advance函数                           *
 *                   根据模板参数推导进行函数重载                  *
 * *************************************************************** */

/* 针对input_iterator，bidirectional_iterator和random_iterator重载的三个__advance函数
 * 根据重载判定，没有与output_iterator_tag和forward_iterator_tag完全匹配的__advance函数，
 * 所以默认调用模板参数InputIterator类型的__advance函数，功能上完全满足要求 */ 
namespace {

    template <class InputIterator, class Distance>
    inline void __advance(InputIterator& it, Distance n, tinystl::input_iterator_tag){
        while(n--)
            ++it;
    }

    template <class BidirectionalIterator, class Distance>
    inline void __advance(BidirectionalIterator& it, Distance n, tinystl::bidirectional_iterator_tag) {
        if(n > 0) 
            while(n--)  ++it;
        else
            while(n++)  ++it;
    }

    template <class RandomAccessIterator, class Distance>
    inline void __advance(RandomAccessIterator& it, Distance n, tinystl::random_access_iterator_tag) {
        it += n;
    }

}
/* advance函数接口，根据迭代器类型选择合适的__advance函数 */
template <class InputIterator, class Distance>
void advance(InputIterator& it, Distance n) {
    /* __advance是函数重载，会根据第三个参数进行模板参数推导，从而选择合适的函数 */
    __advance(it, n, iterator_category(it));
}



/* ********************************************************* *
 *                      distance函数                         *
 *               根据模板参数推导进行函数重载                *
 * ********************************************************* */
namespace {

    template <class InputIterator, class Distance>
    inline void __distance(InputIterator front, 
                           InputIterator back, 
                           Distance& n, 
                           tinystl::input_iterator_tag) {
        while(front != back) {
            ++front;
            ++n;
        }
    }

    template <class RandomAccessIterator, class Distance>
    inline void __distance(RandomAccessIterator front, 
                           RandomAccessIterator back, 
                           Distance& n, 
                           tinystl::random_access_iterator_tag) {
        n += back - front;
    }



    template <class InputIterator>
    inline typename tinystl::iterator_traits<InputIterator>::difference_type
    __distance(InputIterator front, InputIterator back, tinystl::input_iterator_tag) {
        auto n = difference_type(front);
        while(front != back) {
            ++front;
            ++n;
        }
        return n;
    }

    template <class RandomAccessIterator>
    inline typename tinystl::iterator_traits<RandomAccessIterator>::difference_type
    __distance(RandomAccessIterator front, RandomAccessIterator back, tinystl::random_access_iterator_tag) {
        return back - front;
    }

}
/* 将结果保存在引用参数n中 */
template <class InputIterator, class Distance>
void distance(InputIterator front, InputIterator back, Distance& n) {
    __distance(front, back, n, iterator_category(front));
}

/* 将结果返回 */
template <class InputIterator>
inline typename tinystl::iterator_traits<InputIterator>::difference_type
distance(InputIterator front, InputIterator back) {
    return __distance(front, back, iterator_category(front));
}


template <class Iterator>
class reverse_iterator 
{
public:
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<Iterator>::value_type        value_type;
    typedef typename iterator_traits<Iterator>::difference_type   difference_type;
    typedef typename iterator_traits<Iterator>::pointer           pointer;
    typedef typename iterator_traits<Iterator>::reference         reference;
    typedef Iterator                                              iterator_type;
    typedef reverse_iterator<Iterator>                            Self;

protected:
    iterator_type current_;

public:
    reverse_iterator() {}
    explicit reverse_iterator(iterator_type x)
        : current_(x)
    {  }

    template <class U>
    reverse_iterator(const reverse_iterator<U>& other)
        : current_(other.base())
    {  }

    template <class U>
    reverse_iterator& operator=(const reverse_iterator<U>& other)
    { current_ = other.base(); }

    iterator_type base() const
    {
        iterator_type tmp(current_);
        return ++tmp;
    }

    reference operator*() const
    {
        iterator_type tmp(current_);
        return *(--tmp);
    }
    pointer operator->() const
    {
        iterator_type tmp(current_);
        return &(*(--tmp));
    }

    reference operator[](difference_type n) const
    {
        return *(current_ - (n + 1));
    }

    Self& operator++()
    {
        --current_;
        return *this;
    }
    
    Self& operator++(int)
    {
        Self tmp(*this);
        --current_;
        return tmp;
    }

    Self& operator--()
    {
        ++current_;
        return *this;
    }

    Self& operator--(int)
    {
        Self tmp(*this);
        ++current_;
        return tmp;
    }

    Self& operator+=(difference_type n)
    {
        tinystl::advance(current_, -n);
        return *this;
    }

    Self& operator-=(difference_type n)
    {
        return operator+=(-n);
    }

    Self operator+(difference_type n) const
    {
        Self tmp(*this);
        return tmp += n;
    }

    Self operator-(difference_type n) const
    {
        Self tmp(*this);
        return tmp -= n;
    }
};

template <class Iterator1, class Iterator2>
bool operator==(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() == rhs.base();
}

template <class Iterator1, class Iterator2>
bool operator!=(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs)
{
    return !(lhs == rhs);
}

template <class Iterator1, class Iterator2>
bool operator<(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() > rhs.base();
}

template <class Iterator1, class Iterator2>
bool operator>(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() < rhs.base();
}

template <class Iterator1, class Iterator2>
bool operator<=(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs)
{
    return !(lhs > rhs);
}

template <class Iterator1, class Iterator2>
bool operator>=(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs)
{
    return !(lhs < rhs);
}


template <class Iterator>
reverse_iterator<Iterator>
operator+(typename reverse_iterator<Iterator>::difference_type n,
          const reverse_iterator<Iterator>& it)
{
    reverse_iterator<Iterator> tmp(it);
    return tmp += n;
}

template <class Iterator1, class Iterator2>
auto operator-(const reverse_iterator<Iterator1>& lhs,
               const reverse_iterator<Iterator2>& rhs)
{
    return rhs.base() - lhs.base();
}









}
