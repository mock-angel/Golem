
#ifndef _GOLEM_PROFILE_RESULT_H_
#define _GOLEM_PROFILE_RESULT_H_

// #include <iostream>
#include <fstream>
#include <chrono>

class ProfileResult
{
public:
    std::string name;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    std::chrono::duration<double> duration;

    ProfileResult(std::string name, std::chrono::duration<double> duration)
    {
        this->name = name;
        this->duration = duration;
    }

    friend std::ostream &operator<<(std::ostream &os, const ProfileResult &result)
    {
        os << result.name << ": " << result.duration.count() << "ms";
        return os;
    }
};

#endif // _GOLEM_TRACKER_JSON_WRITER_H_