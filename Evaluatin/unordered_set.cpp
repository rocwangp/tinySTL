#include "../Profiler/profiler.h"
#include "../unordered_set.h"
#include "../vector.h"

#include <queue>
#include <functional>
#include <string>
#include <unordered_set>
#include <unistd.h>

using namespace tinystl::Profiler;


class TestBase
{
public:
    virtual ~TestBase() {}
    virtual void run_insert(int count) const = 0;
    virtual void run_count(int count) const = 0;

};

class TestTinystl : public TestBase
{
public:
    virtual ~TestTinystl() {}
    virtual void run_insert(int count) const    
    {
        std::cout << "tinystl::unordered_set<int> for insert " << count << " times" << std::endl;
        tinystl::unordered_set<int> ust;
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            ust.insert(i);
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
    virtual void run_count(int count) const
    {
        std::cout << "tinystl::unordered_set<int> for count " << count << " times" << std::endl;
        tinystl::unordered_set<int> ust; 
        int insertCnt = std::max(10000, count / 10000);
        for(int i = 0; i != insertCnt; ++i)
            ust.insert(i);
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            ust.count(i);
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
};

class TestStd : public TestBase
{
public:
    virtual ~TestStd() {}
    virtual void run_insert(int count) const    
    {
        std::cout << "std::unordered_set<int> for insert " << count << " times" << std::endl;
        std::unordered_set<int> ust;
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            ust.insert(i);
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
    virtual void run_count(int count) const
    {
        std::cout << "std::unordered_set<int> for count " << count << " times" << std::endl;
        std::unordered_set<int> ust; 
        int insertCnt = std::max(10000, count / 10000);
        for(int i = 0; i != insertCnt; ++i)
            ust.insert(i);
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            ust.count(i);
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
};


enum class OpType 
{
    INSERT,
    COUNT
};


static const char* argvOpt = "c:tdiq";

int main(int argc, char** argv)
{
    OpType type = OpType::INSERT;
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
            type = OpType::INSERT;
            break;
        case 'q':
            type = OpType::COUNT;
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
    if(type == OpType::INSERT)
        testObj->run_insert(count);
    else
        testObj->run_count(count);
    delete testObj;
    return 0;
}


