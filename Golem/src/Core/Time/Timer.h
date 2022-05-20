
#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <Core/Tracker/Tracker.h>
#include <Core/Profiler/Profiler.h>

#include "TimerResult.h"

class Tracker;
class TimerResult;

namespace Golem
{
    using namespace std::literals::chrono_literals;

    class Timer
    {
    private:
        // std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
        std::chrono::time_point<std::chrono::steady_clock> m_startTime, m_endTime;
        std::chrono::duration<double, std::micro> m_duration;
        std::string m_name;

        bool m_Stopped = false;

    public:
        Timer(std::string name) : m_name(name), m_duration(0), m_Stopped(false)
        {

            m_startTime = std::chrono::steady_clock::now();
            std::cout << "Timer: " << name << " Started at :" << m_startTime.time_since_epoch().count() << std::endl;
        }
        ~Timer()
        {
            if (!m_Stopped)
                Stop();
        }

        void Stop()
        {
            m_endTime = std::chrono::steady_clock::now();

            std::chrono::duration<double, std::micro> start = std::chrono::duration<double, std::micro>{(m_startTime).time_since_epoch()};
            std::chrono::duration<double, std::micro> end = std::chrono::duration<double, std::micro>((m_endTime).time_since_epoch());
            m_duration = std::chrono::time_point_cast<std::chrono::microseconds>(m_endTime).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_startTime).time_since_epoch();
            std::cout << "Timer: end: " << m_name << "Start, End, Duration: " << start.count() << ", " << end.count() << ", " << m_duration.count() << std::endl;

            // Tracker::GetInstance().AppendTimer({m_name, start, end, m_duration, std::this_thread::get_id()});
            Profiler::GetInstance().WriteProfile({m_name, start, end, m_duration, std::this_thread::get_id()});
            // std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(m_endTime - m_startTime).count() << "ms" << std::endl;
            m_Stopped = true;
        }
    };

}
