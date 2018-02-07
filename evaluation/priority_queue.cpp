#include "../profiler/profiler.h"
#include "../priority_queue.h"
#include "../vector.h"

#include <queue>
#include <functional>
#include <string>
#include <unistd.h>

using namespace tinystl::profiler;


class TestBase
{
public:
    virtual ~TestBase() {}
    virtual void run_push(int count) const = 0;
    virtual void run_pop(int count) const = 0;

};

class TestTinystl : public TestBase
{
public:
    virtual ~TestTinystl() {}
    virtual void run_push(int count) const    
    {
        std::cout << "tinystl::priority_queue<int> for push " << count << " times" << std::endl;
        tinystl::priority_queue<int> pq;
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            pq.push(i);
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
    virtual void run_pop(int count) const
    {
        std::cout << "tinystl::priority_queue<int> for pop " << count << " times" << std::endl;
        tinystl::vector<int> v;
        for(int i = 0; i != count; ++i)
            v.push_back(i);
        tinystl::priority_queue<int> pq(v.begin(), v.end()); 
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            pq.pop();
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
};

class TestStd : public TestBase
{
public:
    virtual ~TestStd() {}
    virtual void run_push(int count) const    
    {
        std::cout << "std::priority_queue<int> for push " << count << " times" << std::endl;
        std::priority_queue<int> pq;
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            pq.push(i);
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
    virtual void run_pop(int count) const
    {
        std::cout << "std::priority_queue<int> for pop " << count << " times" << std::endl;
        tinystl::vector<int> v;
        for(int i = 0; i != count; ++i)
            v.push_back(i);
        std::priority_queue<int> pq(v.begin(), v.end()); 
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            pq.pop();
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
};


enum class OpType 
{
    PUSH,
    POP 
};


static const char* argvOpt = "c:tduo";

int main(int argc, char** argv)
{
    OpType type = OpType::PUSH;
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
        case 'u':
            type = OpType::PUSH;
            break;
        case 'o':
            type = OpType::POP;
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
    if(type == OpType::PUSH)
        testObj->run_push(count);
    else
        testObj->run_pop(count);
    delete testObj;
    return 0;
}

