

#pragma once

#include <string>
#include <chrono>
#include <thread>

struct TimerResult
{
    std::string Name;
    std::chrono::duration<double, std::micro> Start;
    std::chrono::duration<double, std::micro> End;
    std::chrono::microseconds ElapsedTime;
    std::thread::id ThreadID;

    // friend std::ostream &operator<<(std::ostream &os, const TimerData &timerData);

    // friend std::ostream &operator<<(std::ostream &os, const TimerData &timerData)
    // {
    //     os << timerData.m_name << ": " << timerData.m_duration.count() << "ms";
    //     return os;
    // }
};
