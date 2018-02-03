#pragma once

#include <chrono>
#include <ratio>
#include <iostream>

#include <sys/resource.h>
#include <sys/time.h>

namespace tinystl
{
    namespace Profiler
    {
        class ProfilerInstance
        {
        public:
            typedef std::chrono::steady_clock SteadyClock;
            typedef SteadyClock::time_point   TimePoint;
            typedef std::chrono::duration<double, std::ratio<1, 1>> DurationTime;
            enum class MemoryUnit { KB_, MB_, GB_ };
        private:
            static DurationTime kDuringTime;
            static TimePoint kStartTime;
            static TimePoint kFinishTime;
        public:
            static void start();
            static void finish();
            static void dumpDuringTime(std::ostream& os = std::cout);

            static double second();
            static double millisecond();

            static size_t memory(MemoryUnit mu = MemoryUnit::KB_);
        };
    }
}
