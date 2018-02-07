#include "../profiler/profiler.h"
#include "../algorithm.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include <unistd.h>

using namespace tinystl::profiler;

class TestBase
{
public:
    virtual ~TestBase() {}
    virtual void run(int count) const = 0;
};

class TestTinystl : public TestBase
{
public:
    virtual ~TestTinystl() {}
    virtual void run(int count) const
    {
        std::cout << "tinystl::algorithm for sort " << count << " times" << std::endl;
        std::random_device rd;
        std::vector<int> v(count);
        std::generate(v.begin(), v.end(), [&rd](){ return rd(); });
        ProfilerInstance::start();
        tinystl::sort(v.begin(), v.end());
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
};

class TestStd : public TestBase
{
public:
    virtual ~TestStd() {}
    virtual void run(int count) const
    {
        std::cout << "std::algorithm for sort " << count << " times" << std::endl;
        std::random_device rd;
        std::vector<int> v(count);
        std::generate(v.begin(), v.end(), [&rd](){ return rd(); });
        ProfilerInstance::start();
        std::sort(v.begin(), v.end());
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
};



const char* argvOpt = "c:td";

int main(int argc, char** argv)
{
    TestBase* testObj = nullptr;
    int count = 1000000;
    int oc;
    while((oc = ::getopt(argc, argv, argvOpt)) != -1)
    {
        switch(oc)
        {
        case 't':
            testObj = new TestTinystl();
            break;
        case 'd':
            testObj = new TestStd();
            break;
        case 'c':
            count = ::strtol(optarg, nullptr, 10);
            break;
        }
    }
    if(testObj == nullptr)
    {
        std::cerr << "parameter error" << std::endl;
        return 1;
    }
    testObj->run(count);
    delete testObj;
    return 0;
}


