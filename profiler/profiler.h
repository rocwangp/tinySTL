#pragma once

#include <chrono>
#include <ratio>
#include <iostream>

#include <sys/resource.h>
#include <sys/time.h>

namespace tinystl
{
    namespace profiler
    {
        template <int inst>
        class Profiler
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
            static void start()
            {
                kStartTime = SteadyClock::now();
            }
            static void finish()
            {
                kFinishTime = SteadyClock::now();
                kDuringTime = std::chrono::duration_cast<DurationTime>(kFinishTime - kStartTime);
            }
            static void dumpDuringTime(std::ostream& os = std::cout)
            {
                os << "total " << kDuringTime.count() * 1000 << " milliseconds" << std::endl;
            }

            static double second()
            {
                return kDuringTime.count();
            }
            static double millisecond()
            {
                return kDuringTime.count() * 1000;
            }

            static size_t memory(MemoryUnit mu = MemoryUnit::KB_)
            {
                size_t memory = 0;
                struct rusage usage;
                if(::getrusage(RUSAGE_SELF, &usage) == -1)
                    throw std::runtime_error("getrusage failed");
                memory = usage.ru_maxrss / 1024;
                switch(mu)
                {
                case MemoryUnit::KB_:
                    memory = memory / 1024;
                    break;
                case MemoryUnit::MB_:
                    memory = memory / 1024 / 1024;
                    break;
                case MemoryUnit::GB_:
                    memory = memory / 1024 / 1024 / 1024;
                    break;
                }
                return memory;
            }
        };
        template <int inst>
        typename Profiler<inst>::DurationTime Profiler<inst>::kDuringTime;
        template <int inst>
        typename Profiler<inst>::TimePoint Profiler<inst>::kStartTime;
        template <int inst>
        typename Profiler<inst>::TimePoint Profiler<inst>::kFinishTime;
        
        using ProfilerInstance = Profiler<0>;
    }
}
