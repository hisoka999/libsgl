#pragma once

#include <string>
#include <chrono>

namespace utils
{
    class PerformanceLogger
    {
    private:
        std::string m_scope;
        std::chrono::time_point<std::chrono::system_clock> m_startTime;
        std::chrono::time_point<std::chrono::system_clock> m_lastStepTime;

    public:
        PerformanceLogger(const std::string &scope);
        void step(const std::string &step);
        void end();
        ~PerformanceLogger();
    };

} // namespace utils