#include "../profiler/profiler.h"
#include "../deque.h"

#include <deque>
#include <iostream>
#include <string>
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
        std::cout << "tinystl::deque for push_front/back " << count << " times" << std::endl;
        tinystl::deque<int> dq;
        ProfilerInstance::start();
        const int max = count;
        for(int i = 0; i != max / 2; ++i)
            dq.push_front(i);
        for(int i = 0; i != max; ++i)
            dq.push_back(i);
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
        std::cout << "std::deque for push_front/back " << count << " times" << std::endl;
        std::deque<int> dq;
        ProfilerInstance::start();
        const int max = count;
        for(int i = 0; i != max / 2; ++i)
            dq.push_front(i);
        for(int i = 0; i != max; ++i)
            dq.push_back(i);
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

