
#ifndef _GOLEM_TRACKER_TRACKER_H_
#define _GOLEM_TRACKER_TRACKER_H_

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>

#include <Core/Time/TimerResult.h>

class Tracker
{
private:
    std::vector<TimerResult> m_timers;
    std::map<std::string, std::list<TimerResult>> m_trackerData;
    // TrackerJsonWriter jsonWriter;

public:
    static Tracker instance;
    static Tracker &GetInstance()
    {

        return instance;
    }

    void AppendTimer(const TimerResult &timer)
    {
        // std::cout << "Timer: end: append: " << timer.m_name << " " << m_timers.size() << std::endl;
        m_timers.push_back(timer);
        m_trackerData[timer.Name].push_back(timer);

        // std::cout << "Timer: end: append: " << timer.m_name << " " << m_timers.size() << std::endl;
    }

    void ClearTimers()
    {
        m_timers.clear();
    }

    std::vector<TimerResult> *GetTimers()
    {
        return &m_timers;
    }

    std::map<std::string, std::list<TimerResult>> *GetTrackerData()
    {
        return &m_trackerData;
    }

    Tracker(/* args */);
    ~Tracker();
};

#endif // _GOLEM_TRACKER_TRACKER_H_
