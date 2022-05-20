#pragma once

#include <Core/Time/TimerResult.h>

namespace Golem
{

    // Class used to profile data
    class Profiler
    {
        static Profiler Instance;

        std::string m_FileName = "Profile.json";

    public:
        Profiler *GetInstance() { return &Instance; }

        // Write a profile to file
        void WriteProfile(const TimerResult &timerResult)
        {
        }
    };

} // namespace Golem
