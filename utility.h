#pragma once


namespace tinystl
{

template <class Key, class Value>
struct pair
{
    typedef Key         key_type;
    typedef Value       value_type;

    pair() {}

    pair(const key_type& key, const value_type& value)
        : first(key), second(value)
    {  }

    pair(const pair& other)
        : first(other.first), second(other.second)
    {  }

    pair& operator=(const pair& other)
    {
        first = other.first;
        second = other.second;
    }

    key_type first;
    value_type second;
};

template <class Key, class Value>
pair<Key, Value> make_pair(const Key& key, const Value& value)
{
    return tinystl::pair<Key, Value>(key, value);
}

template <class Key, class Value>
bool operator==(const pair<Key, Value>& lhs, const pair<Key, Value>& rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <class Key, class Value>
bool operator!=(const pair<Key, Value>& lhs, const pair<Key, Value>& rhs)
{
    return !(lhs == rhs);
}

}
