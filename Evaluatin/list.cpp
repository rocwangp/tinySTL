#include "../Profiler/profiler.h"
#include "../list.h"

#include <list>
#include <functional>
#include <string>
#include <unistd.h>

#include <random>

using namespace tinystl::Profiler;


class TestBase
{
public:
    virtual ~TestBase() {}
    virtual void run_push_back(int count) const = 0;
    void run_sort(int count, int times = 100) const 
    {
        tinystl::list<int> list1;
        std::list<int> list2;
        std::default_random_engine dre;
        std::uniform_int_distribution<int> id;
        for(int i = 0; i != count; ++i)
        {
            auto n = id(dre);
            list1.push_back(n);
            list2.push_back(n);
        }
        double cost1 = 0.0, cost2 = 0.0;
        for(int i = 0; i != times; ++i)
        {
            ProfilerInstance::start();
            list1.sort();
            ProfilerInstance::finish();
            cost1 += ProfilerInstance::millisecond();

            ProfilerInstance::start();
            list2.sort();
            ProfilerInstance::finish();
            cost2 += ProfilerInstance::millisecond();
        }
        std::cout << "tinystl::list<int>::sort time: " << cost1  << "ms" << std::endl;
        std::cout << "std::list<int>::sort time: " << cost2  << "ms" << std::endl;
    }

};

class TestTinystl : public TestBase
{
public:
    virtual ~TestTinystl() {}
    virtual void run_push_back(int count) const    
    {
        std::cout << "tinystl::list<int> for push_back " << count << " times" << std::endl;
        tinystl::list<int> l;
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            l.push_back(i);
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
};

class TestStd : public TestBase
{
public:
    virtual ~TestStd() {}
    virtual void run_push_back(int count) const    
    {
        std::cout << "std::list<int> for push_back " << count << " times" << std::endl;
        std::list<int> l;
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            l.push_back(i);
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
};


enum class OpType 
{
    PUSH_BACK,
    SORT
};


static const char* argvOpt = "c:tdps";

int main(int argc, char** argv)
{
    OpType type = OpType::PUSH_BACK;
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
        case 'p':
            type = OpType::PUSH_BACK;
            break;
        case 's':
            type = OpType::SORT;
            break;
        case 'c':
            count = ::strtol(optarg, nullptr, 10);
            break;
        case ':':
            std::cerr << "parameter error";
            return 1;
        }
    }
    if(!testObj)
    {
        std::cerr << "paramter error";
        return 1;
    }
    if(type == OpType::PUSH_BACK)
        testObj->run_push_back(count);
    else
        testObj->run_sort(count);
    delete testObj;
    return 0;
}


