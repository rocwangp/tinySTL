#include "profiler.h"

namespace tinystl
{
    namespace Profiler
    {
        
        ProfilerInstance::DurationTime ProfilerInstance::kDuringTime;
        ProfilerInstance::TimePoint ProfilerInstance::kStartTime;
        ProfilerInstance::TimePoint ProfilerInstance::kFinishTime;

        void ProfilerInstance::start()
        {
            kStartTime = SteadyClock::now();
        }

        void ProfilerInstance::finish()
        {
            kFinishTime = SteadyClock::now();
            kDuringTime = std::chrono::duration_cast<DurationTime>(kFinishTime - kStartTime);
        }

        void ProfilerInstance::dumpDuringTime(std::ostream& os)
        {
            os << "total " << kDuringTime.count() * 1000 << " milliseconds" << std::endl;
        }

        double ProfilerInstance::second()
        {
            return kDuringTime.count();
        }

        double ProfilerInstance::millisecond()
        {
            return kDuringTime.count() * 1000;
        }

        size_t ProfilerInstance::memory(MemoryUnit mu)
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
}

