#pragma once

#include "basic_string.h"
#include "functional.h"


namespace tinystl
{

typedef tinystl::basic_string<char> string;

int stoi(const tinystl::string& str, std::size_t* pos = 0, int base = 10);
long stol(const tinystl::string& str, std::size_t* pos, int base = 10);
long long stoll(const tinystl::string& str, std::size_t* pos, int base = 10);
unsigned long stoul(const tinystl::string& str, std::size_t* pos = 0, int base = 10);
unsigned long long stoull(const tinystl::string& str, std::size_t* pos = 0, int base = 10);
float stof(const tinystl::string& str, std::size_t* pos = 0);
double stod(const tinystl::string& str, std::size_t* pos = 0);
long double stold(const tinystl::string& str, std::size_t* pos = 0);

tinystl::string to_string(int value);
tinystl::string to_string(long value);
tinystl::string to_string(long long value);
tinystl::string to_string(unsigned int value);
tinystl::string to_string(unsigned long value);
tinystl::string to_string(unsigned long long value);
tinystl::string to_string(float value);
tinystl::string to_string(double value);
tinystl::string to_string(long double value);





}
