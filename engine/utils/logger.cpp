#include "engine/utils/logger.h"
#include <iostream>
#include <chrono>
#include <ctime>

namespace utils
{
    const std::string get_loglevel(LogLevel pLevel)
    {
        switch (pLevel)
        {
        case LogLevel::error:
            return "\e[31merror\e[0m";
        case LogLevel::info:
            return "\e[34minfo\e[0m";
        case LogLevel::trace:
            return "\e[32mtrace\e[0m";
        case LogLevel::warn:
            return "\e[33mwarn\e[0m";
        default:
            return "";
        }
    }

    Logger::Logger(const std::string &loggerName, LogLevel pLevel) : m_loggerName(loggerName), level(pLevel)
    {
    }

    Logger::Logger() : m_loggerName("default"), level(LogLevel::trace) //, out(std::cerr)
    {
    }

    void Logger::logSDLError(const std::string &msg)
    {
        log(LogLevel::error, msg, SDL_GetError());
    }

    Logger::~Logger()
    {
        // dtor
    }

    void Logger::log(LogLevel pLevel, const std::string &src,
                     const std::string &msg)
    {
        if (level < pLevel)
            return;

        char mbstr[100];
        auto time = std::chrono::system_clock::now();
        std::time_t time_t = std::chrono::system_clock::to_time_t(time);
        std::strftime(mbstr, sizeof(mbstr), "%H:%M:%S", std::localtime(&time_t));

        getStreamWriter() << mbstr << "[" << get_loglevel(pLevel) << "]" << m_loggerName << "::" << src << ":"
                          << msg << std::endl;
    }

    std::ostream &Logger::getStreamWriter()
    {
#ifdef SGL_DEBUG
        return std::cout;
#else
        return out;
#endif
    }

    void Logger::error(const std::string &src, const std::string &msg)
    {
        log(LogLevel::error, src, msg);
    }
    void Logger::warn(const std::string &src, const std::string &msg)
    {
        log(LogLevel::warn, src, msg);
    }
    void Logger::info(const std::string &src, const std::string &msg)
    {
        log(LogLevel::info, src, msg);
    }
    void Logger::trace(const std::string &src, const std::string &msg)
    {
        log(LogLevel::trace, src, msg);
    }

} // namespace utils
