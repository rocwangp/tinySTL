#pragma once

#include "basic_string.h"
#include "functional.h"


namespace tinystl
{

typedef basic_string<char> string;


/* int stoi(const string& str, std::size_t* pos = 0, int base = 10) */
/* { */
/*     *pos = str.size(); */
/*     return  std::strtol(str.c_str(), nullptr, base); */
/* } */

/* long stol(const string& str, std::size_t* pos, int base = 10) */
/* { */
/*     *pos = str.size(); */
/*     return std::strtol(str.c_str(), nullptr, base); */
/* } */

/* long long stoll(const string& str, std::size_t* pos, int base = 10) */
/* { */
/*     *pos = str.size(); */
/*     return std::strtoll(str.c_str(), nullptr, base); */
/* } */


/* unsigned long stoul(const string& str, std::size_t* pos = 0, int base = 10) */
/* { */
/*     *pos = str.size(); */
/*     return std::strtoul(str.c_str(), nullptr, base); */
/* } */

/* unsigned long long stoull(const string& str, std::size_t* pos = 0, int base = 10) */
/* { */
/*     *pos = str.size(); */
/*     return std::strtoull(str.c_str(), nullptr, base); */
/* } */

/* float stof(const string& str, std::size_t* pos = 0) */
/* { */
/*     *pos = str.size(); */
/*     return std::strtof(str.c_str(), nullptr); */
/* } */

/* double stod(const string& str, std::size_t* pos = 0) */
/* { */
/*     *pos = str.size(); */
/*     return std::strtod(str.c_str(), nullptr); */
/* } */

/* long double stold(const string& str, std::size_t* pos = 0) */
/* { */
/*     *pos = str.size(); */
/*     return std::strtold(str.c_str(), nullptr); */
/* } */

/* string to_string(int value) */
/* { */
/*     char buf[1024]; */
/*     std::sprintf(buf, "%d", value); */
/*     return string(buf); */
/* } */

/* string to_string(long value) */
/* { */
/*     char buf[1024]; */
/*     std::sprintf(buf, "%ld", value); */
/*     return string(buf); */
/* } */
/* string to_string(long long value) */
/* { */
/*     char buf[1024]; */
/*     std::sprintf(buf, "%lld", value); */
/*     return string(buf); */
/* } */
/* string to_string(unsigned value) */
/* { */
/*     char buf[1024]; */
/*     std::sprintf(buf, "%u", value); */
/*     return string(buf); */
/* } */
/* string to_string(unsigned long value) */
/* { */
/*     char buf[1024]; */
/*     std::sprintf(buf, "%lu", value); */
/*     return string(buf); */
/* } */
/* string to_string(unsigned long long value) */
/* { */
/*     char buf[1024]; */
/*     std::sprintf(buf, "%llu", value); */
/*     return string(buf); */
/* } */


/* string to_string(float value) */
/* { */
/*     char buf[1024]; */
/*     std::sprintf(buf, "%f", value); */
/*     return string(buf); */
/* } */

/* string to_string(double value) */
/* { */
/*     char buf[1024]; */
/*     std::sprintf(buf, "%f", value); */
/*     return string(buf); */
/* } */


/* string to_string(long double value) */
/* { */
/*     char buf[1024]; */
/*     std::sprintf(buf, "%Lf", value); */
/*     return string(buf); */
/* } */



}
