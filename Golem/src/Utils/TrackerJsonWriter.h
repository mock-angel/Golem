
#ifndef _GOLEM_TRACKER_JSON_WRITER_H_
#define _GOLEM_TRACKER_JSON_WRITER_H_

// #include <iostream>
#include <fstream>
#include <ProfileResult.h>

class TrackerJsonWriter
{
private:
    std::ofstream m_OutputFileStream;
    std::string m_SessionName;
    std::string m_OutputFilePath;
    void WriteHeader()
    {
        m_OutputFileStream << "{" << std::endl;
        m_OutputFileStream << "\"session\": \"" << m_SessionName << "\"," << std::endl;
        m_OutputFileStream << "\"tracker\": [" << std::endl;
    }
    void WriteFooter()
    {
        m_OutputFileStream << "]" << std::endl;
        m_OutputFileStream << "}" << std::endl;
    }

public:
    void BeginSession(const std::string &sessionName, const std::string &filepath = "debug.json")
    {
        m_OutputFilePath = filepath;
        m_OutputFileStream.open(filepath);
        WriteHeader();
    }

    void EndSession()
    {
        WriteFooter();
        m_OutputFileStream.close();
    }

    void WriteProfile(const ProfileResult &result)
    {
        m_OutputFileStream << "{" << std::endl;
        m_OutputFileStream << "\"cat\": \"" << result.name << "\"," << std::endl;
        m_OutputFileStream << "\"dur\": " << result.duration.count() << std::endl;
        m_OutputFileStream << "\"name\": " << result.name << std::endl;
        m_OutputFileStream << "\"ph\": " << result.start.time_since_epoch().count() << std::endl;
        m_OutputFileStream << "\"pid\": " << result.duration.count() << std::endl;
        m_OutputFileStream << "\"tid\": " << result.duration.count() << std::endl;
        m_OutputFileStream << "\"ts\": " << result.end.time_since_epoch().count() << std::endl;
        m_OutputFileStream << "}," << std::endl;
    }
};

#endif // _GOLEM_TRACKER_JSON_WRITER_H_