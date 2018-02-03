#include "../string.h"
#include "../Profiler/profiler.h"

#include <iostream>
#include <string>
#include <unistd.h>

using namespace tinystl::Profiler;

class TestBase
{
public:
    virtual ~TestBase() {}
    virtual void run(int count, char value) const = 0;
};

class TestTinystl : public TestBase
{
public:
    virtual ~TestTinystl() {}
    virtual void run(int count, char value) const
    {
        std::cout << "tinystl::string for push_back " << count << " times" << std::endl;
        tinystl::string str;
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            str.push_back(value);
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
};

class TestStd : public TestBase
{
public:
    virtual ~TestStd() {}
    virtual void run(int count, char value) const
    {
        std::cout << "std::string for push_back " << count << " times" << std::endl;
        std::string str;
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            str.push_back(value);
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
    testObj->run(count, 'c');
    delete testObj;
    return 0;
}
