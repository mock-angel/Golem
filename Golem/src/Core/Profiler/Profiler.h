#pragma once

#include <fstream>
#include <memory>
#include <iomanip>

#include <Core/Time/TimerResult.h>

namespace Golem
{

    struct ProfilerSession
    {
        std::string Name;
        std::string FilePath;
    };

    // Class used to profile data
    class Profiler
    {
        static Profiler Instance;

        std::string m_FileName = "Profile.json";

        std::unique_ptr<ProfilerSession> m_CurrentSession;

    public:
        static Profiler &GetInstance() { return Instance; }

        Profiler(){};
        ~Profiler(){};

        void StartSession(const std::string &name, const std::string &filepath = "Profile.json")
        {
            if (m_CurrentSession)
            {
                EndSession();
            }
            m_FileName = filepath;
            m_OutputStream.open(m_FileName);

            WriteHeader();
        }

        // Ends current session provided one has already been started
        void EndSession()
        {
            WriteFooter();
            m_OutputStream.close();
            m_CurrentSession.reset();
        }

        // Write a profile result to file
        void WriteProfile(const TimerResult &timerResult)
        {
            // if (m_CurrentSession)
            // {
            m_OutputStream << std::setprecision(3) << std::fixed;
            m_OutputStream << ",{";
            m_OutputStream << "\"cat\":\"function\",";
            m_OutputStream << "\"dur\":" << timerResult.ElapsedTime.count() << ',';
            m_OutputStream << "\"name\":\"" << timerResult.Name << "\",";
            m_OutputStream << "\"ph\":\"X\",";
            m_OutputStream << "\"pid\":0,";
            m_OutputStream << "\"tid\":" << timerResult.ThreadID << ",";
            m_OutputStream << "\"ts\":" << timerResult.Start.count() << "}";
            m_OutputStream << std::endl;
            // }
        }

    private:
        std::ofstream m_OutputStream;

        // Writes header to json file.
        // FileProfiler.
        // TODO: Seperate to different class if required
        void WriteHeader()
        {
            m_OutputStream << "{\"otherData\": {},";
            m_OutputStream << "\"traceEvents\":[{}";
            m_OutputStream.flush();
        }

        // Writes footer to json file.
        // FileProfiler.
        // TODO: Seperate to different class if required
        void WriteFooter()
        {
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }
    };

} // namespace Golem
