#pragma once

#include "basic_string.h"
#include "functional.h"


namespace tinystl
{

typedef tinystl::basic_string<char> string;


inline int stoi(const tinystl::string& str, std::size_t* pos , int base )
{
    *pos = str.size();
    return  std::strtol(str.c_str(), nullptr, base);
}

inline long stol(const tinystl::string& str, std::size_t* pos, int base )
{
    *pos = str.size();
    return std::strtol(str.c_str(), nullptr, base);
}

inline long long stoll(const tinystl::string& str, std::size_t* pos, int base )
{
    *pos = str.size();
    return std::strtoll(str.c_str(), nullptr, base);
}


inline unsigned long stoul(const tinystl::string& str, std::size_t* pos , int base )
{
    *pos = str.size();
    return std::strtoul(str.c_str(), nullptr, base);
}

inline unsigned long long stoull(const tinystl::string& str, std::size_t* pos, int base)
{
    *pos = str.size();
    return std::strtoull(str.c_str(), nullptr, base);
}

inline float stof(const tinystl::string& str, std::size_t* pos)
{
    *pos = str.size();
    return std::strtof(str.c_str(), nullptr);
}

inline double stod(const tinystl::string& str, std::size_t* pos)
{
    *pos = str.size();
    return std::strtod(str.c_str(), nullptr);
}

inline long double stold(const tinystl::string& str, std::size_t* pos)
{
    *pos = str.size();
    return std::strtold(str.c_str(), nullptr);
}



inline tinystl::string to_string(int value)
{
    char buf[1024];
    std::sprintf(buf, "%d", value);
    return tinystl::string(buf);
}

inline tinystl::string to_string(long value)
{
    char buf[1024];
    std::sprintf(buf, "%ld", value);
    return tinystl::string(buf);
}
inline tinystl::string to_string(long long value)
{
    char buf[1024];
    std::sprintf(buf, "%lld", value);
    return tinystl::string(buf);
}
inline tinystl::string to_string(unsigned int value)
{
    char buf[1024];
    std::sprintf(buf, "%u", value);
    return tinystl::string(buf);
}
inline tinystl::string to_string(unsigned long value)
{
    char buf[1024];
    std::sprintf(buf, "%lu", value);
    return tinystl::string(buf);
}
inline tinystl::string to_string(unsigned long long value)
{
    char buf[1024];
    std::sprintf(buf, "%llu", value);
    return tinystl::string(buf);
}


inline tinystl::string to_string(float value)
{
    char buf[1024];
    std::sprintf(buf, "%f", value);
    return tinystl::string(buf);
}

inline tinystl::string to_string(double value)
{
    char buf[1024];
    std::sprintf(buf, "%f", value);
    return tinystl::string(buf);
}


inline tinystl::string to_string(long double value)
{
    char buf[1024];
    std::sprintf(buf, "%Lf", value);
    return tinystl::string(buf);
}




}
