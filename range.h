#pragma once

#include <iostream>
#include <stdexcept>
#include <cstdint>

namespace tinystl
{

    template <typename T>
    class range_iterator
    {
        public:
            typedef T               value_type;
            typedef std::size_t     size_type;
            typedef T&              reference;
            typedef const T&        const_reference;
            typedef range_iterator<T> self;

            range_iterator(value_type current, value_type step_len, size_type steps)
                : current_(current),
                  step_len_(step_len),
                  steps_(steps)
            {
            }
            range_iterator(const range_iterator& other)
                : current_(other.current_),
                  step_len_(other.step_len_),
                  steps_(other.steps_)
            {
            }

            reference operator*() {
                return current_;
            }
            const_reference operator*() const {
                return current_;
            }
            self& operator++() {
                current_ += step_len_;
                --steps_;
                return *this;
            }
            self operator++(int) {
                self tmp = *this;
                current_ += step_len_;
                --steps_;
                return tmp;
            }
            bool operator==(const self& other) {
                return steps_ == other.steps_;
            }
            bool operator!=(const self& other) {
                return !operator==(other);
            }
        private:
            value_type current_;
            value_type step_len_;
            size_type steps_;
    };

    template <typename T>
    class range_impl
    {
        public:
            typedef T                    value_type;
            typedef std::size_t          size_type;
            typedef range_iterator<T>    iterator;

            range_impl(value_type begin, value_type end, value_type step_len)
                : begin_(begin),
                  end_(end),
                  step_len_(step_len)
            {
                if(begin_ > end_ && step_len_ > 0) {
                    throw std::runtime_error("begin is greater than end");
                }
                else if(begin_ < end_ && step_len_ <= 0) {
                    throw std::runtime_error("step len is less or equal then 0");
                }
                steps_ = (end_ - begin_) / step_len_;
                if(begin_ + step_len_ * steps_ != end_) {
                    ++steps_;
                }
            }

            iterator begin() { return iterator(begin_, step_len_, steps_); }
            iterator end() { return iterator(end_, step_len_, 0); }
        private:
            value_type begin_;
            value_type end_;
            value_type step_len_;
            size_type steps_;

    };

    template <typename T, typename U>
    auto range(T begin, T end, U step_len) {
        return range_impl<decltype(begin + end + step_len)>(begin, end, step_len);
    }

    template <typename T>
    auto range(T begin, T end) {
        return range_impl<T>(begin, end, 1);
    }

    template <typename T>
    auto range(T end) {
        return range_impl<T>(T(), end, 1);
    }
}
