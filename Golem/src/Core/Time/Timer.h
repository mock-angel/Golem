
#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <Core/Tracker/Tracker.h>

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
        std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime, m_endTime;
        std::chrono::microseconds m_duration;
        std::string m_name;

        bool m_Stopped = false;

    public:
        Timer(std::string name) : m_name(name), m_duration(0), m_Stopped(false)
        {
            std::cout << "Timer: " << name << " Started" << std::endl;
            m_startTime = std::chrono::high_resolution_clock::now();
        }
        ~Timer()
        {
            if (!m_Stopped)
                Stop();
        }

        void Stop()
        {
            m_endTime = std::chrono::high_resolution_clock::now();

            std::chrono::microseconds start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTime).time_since_epoch();
            std::chrono::microseconds end = std::chrono::time_point_cast<std::chrono::microseconds>(m_endTime).time_since_epoch();
            m_duration = end - start;
            std::cout << "Timer: end: " << m_name << ", Duration: " << m_duration.count() << std::endl;

            Tracker::GetInstance().AppendTimer({m_name, start, end, m_duration, std::this_thread::get_id()});
            // Tracker::GetInstance().WriteProfile({m_duration})
            // std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(m_endTime - m_startTime).count() << "ms" << std::endl;
            m_Stopped = true;
        }
    };

}
