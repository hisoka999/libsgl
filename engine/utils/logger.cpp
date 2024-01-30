#include "engine/utils/logger.h"
#include <chrono>
#include <ctime>
#include <iostream>

namespace utils
{
    const std::string get_loglevel(LogLevel pLevel)
    {
#ifdef _WIN32
        switch (pLevel)
        {
            case LogLevel::error:
                return "error";
            case LogLevel::info:
                return "info";
            case LogLevel::trace:
                return "trace";
            case LogLevel::warn:
                return "warn";
            default:
                return "";
        }
#elif !defined(SGL_DEBUG)
        switch (pLevel)
        {
            case LogLevel::error:
                return "error";
            case LogLevel::info:
                return "info";
            case LogLevel::trace:
                return "trace";
            case LogLevel::warn:
                return "warn";
            default:
                return "";
        }
#else
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
#endif
    }

    Logger::Logger(const std::string &loggerName, LogLevel pLevel) : m_loggerName(loggerName), level(pLevel) {}

    void Logger::logSDLError(const std::string &msg) { log(LogLevel::error, msg, SDL_GetError()); }

    Logger::~Logger()
    {

        std::cerr << "logger " << m_loggerName << " destroyed" << std::endl;
#ifndef SGL_DEBUG
        out.flush();
        out.close();
#endif
    }

    void Logger::init(const std::filesystem::path &outputDir, const LogLevel pLevel)
    {
        level = pLevel;
        if (!std::filesystem::exists(outputDir))
            std::filesystem::create_directories(outputDir);

        std::string fileName = m_loggerName + "_log_" +
                               std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".log";
        std::filesystem::path filePath = outputDir / fileName;

#ifndef SGL_DEBUG
        out = {filePath, out.out};
        if (!out.is_open())
        {
            std::cerr << "could not open log file " + fileName << std::endl;
            throw std::runtime_error("could not open log file " + fileName);
        }
#endif
    }

    void Logger::log(LogLevel pLevel, const std::string &src, const std::string &msg)
    {
        if (int(level) < int(pLevel))
            return;

        char mbstr[100];
        const auto time = std::chrono::system_clock::now();
        const std::time_t time_t = std::chrono::system_clock::to_time_t(time);
        std::strftime(mbstr, sizeof(mbstr), "%H:%M:%S", std::localtime(&time_t));

        getStreamWriter() << mbstr << "[" << get_loglevel(pLevel) << "]" << m_loggerName << "::" << src << ":" << msg
                          << std::endl;
    }

    std::ostream &Logger::getStreamWriter()
    {
#ifdef SGL_DEBUG
        return std::cout;
#else
        return out;
#endif
    }

    void Logger::error(const std::string &src, const std::string &msg) { log(LogLevel::error, src, msg); }
    void Logger::warn(const std::string &src, const std::string &msg) { log(LogLevel::warn, src, msg); }
    void Logger::info(const std::string &src, const std::string &msg) { log(LogLevel::info, src, msg); }
    void Logger::trace(const std::string &src, const std::string &msg) { log(LogLevel::trace, src, msg); }

} // namespace utils
