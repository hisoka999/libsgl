#include "PerformanceLogger.h"
#include "logger.h"

namespace utils
{
    PerformanceLogger::PerformanceLogger(const std::string &scope)
        : m_scope(scope), m_startTime(std::chrono::system_clock::now()), m_lastStepTime(m_startTime)
    {
        SGL_LOG_TRACE("start %s", m_scope);
    }

    void PerformanceLogger::step(const std::string &step)
    {
        auto end = std::chrono::system_clock::now();
        auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_lastStepTime).count();

        SGL_LOG_TRACE("end %s(%s) in %i ms", m_scope, step, durationMs);
        m_lastStepTime = std::chrono::system_clock::now();
    }

    void PerformanceLogger::end()
    {
        auto end = std::chrono::system_clock::now();
        auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_startTime).count();

        SGL_LOG_TRACE("end %s in %i ms", m_scope, durationMs);
    }

    PerformanceLogger::~PerformanceLogger()
    {
    }
}