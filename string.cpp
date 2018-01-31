#include "string.h"

namespace tinystl
{


int stoi(const tinystl::string& str, std::size_t* pos , int base )
{
    *pos = str.size();
    return  std::strtol(str.c_str(), nullptr, base);
}

long stol(const tinystl::string& str, std::size_t* pos, int base )
{
    *pos = str.size();
    return std::strtol(str.c_str(), nullptr, base);
}

long long stoll(const tinystl::string& str, std::size_t* pos, int base )
{
    *pos = str.size();
    return std::strtoll(str.c_str(), nullptr, base);
}


unsigned long stoul(const tinystl::string& str, std::size_t* pos , int base )
{
    *pos = str.size();
    return std::strtoul(str.c_str(), nullptr, base);
}

unsigned long long stoull(const tinystl::string& str, std::size_t* pos, int base) 
{
    *pos = str.size();
    return std::strtoull(str.c_str(), nullptr, base);
}

float stof(const tinystl::string& str, std::size_t* pos)
{
    *pos = str.size();
    return std::strtof(str.c_str(), nullptr);
}

double stod(const tinystl::string& str, std::size_t* pos)
{
    *pos = str.size();
    return std::strtod(str.c_str(), nullptr);
}

long double stold(const tinystl::string& str, std::size_t* pos)
{
    *pos = str.size();
    return std::strtold(str.c_str(), nullptr);
}


tinystl::string to_string(int value)
{
    char buf[1024];
    std::sprintf(buf, "%d", value);
    return tinystl::string(buf);
}

tinystl::string to_string(long value)
{
    char buf[1024];
    std::sprintf(buf, "%ld", value);
    return tinystl::string(buf);
}
tinystl::string to_string(long long value)
{
    char buf[1024];
    std::sprintf(buf, "%lld", value);
    return tinystl::string(buf);
}
tinystl::string to_string(unsigned int value)
{
    char buf[1024];
    std::sprintf(buf, "%u", value);
    return tinystl::string(buf);
}
tinystl::string to_string(unsigned long value)
{
    char buf[1024];
    std::sprintf(buf, "%lu", value);
    return tinystl::string(buf);
}
tinystl::string to_string(unsigned long long value)
{
    char buf[1024];
    std::sprintf(buf, "%llu", value);
    return tinystl::string(buf);
}


tinystl::string to_string(float value)
{
    char buf[1024];
    std::sprintf(buf, "%f", value);
    return tinystl::string(buf);
}

tinystl::string to_string(double value)
{
    char buf[1024];
    std::sprintf(buf, "%f", value);
    return tinystl::string(buf);
}


tinystl::string to_string(long double value)
{
    char buf[1024];
    std::sprintf(buf, "%Lf", value);
    return tinystl::string(buf);
}



}



