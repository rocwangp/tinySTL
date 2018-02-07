#include "../profiler/profiler.h"
#include "../vector.h"

#include <vector>
#include <functional>
#include <string>
#include <unistd.h>

using namespace tinystl::profiler;


class TestBase
{
public:
    virtual ~TestBase() {}
    virtual void run(int count, int value) const = 0;
    virtual void run(int count, std::string value) const = 0;

};

class TestTinystl : public TestBase
{
public:
    virtual ~TestTinystl() {}
    virtual void run(int count, int value) const    
    {
        std::cout << "tinystl::vector<int> for push_back " << count << " times" << std::endl;
        tinystl::vector<int> vec;
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            vec.push_back(value);
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
    virtual void run(int count, std::string value) const
    {
        std::cout << "tinystl::vector<string> for push_back " << count << " times" << std::endl;
        tinystl::vector<std::string> vec;
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            vec.push_back(value);
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
};

class TestStd : public TestBase
{
public:
    virtual ~TestStd() {}
    virtual void run(int count, int value) const    
    {
        std::cout << "std::vector<int> for push_back " << count << " times" << std::endl;
        std::vector<int> vec;
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            vec.push_back(value);
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
    virtual void run(int count, std::string value) const
    {
        std::cout << "std::vector<string> for push_back " << count << " times" << std::endl;
        std::vector<std::string> vec;
        ProfilerInstance::start();
        for(int i = 0; i != count; ++i)
            vec.push_back(value);
        ProfilerInstance::finish();
        ProfilerInstance::dumpDuringTime();
    }
};


enum class TestType
{
    INT,
    STRING
};


static const int intValue = 10000000;
static const std::string strValue = "c++::vector";
static const char* argvOpt = "c:tdis";

int main(int argc, char** argv)
{
    TestType type = TestType::INT;
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
        case 'i':
            type = TestType::INT;
            break;
        case 's':
            type = TestType::STRING;
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
    if(type == TestType::INT)
        testObj->run(count, intValue);
    else
        testObj->run(count, strValue);
    delete testObj;
    return 0;
}
