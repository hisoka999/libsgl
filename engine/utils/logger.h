#ifndef UTILS_LOGGER_H
#define UTILS_LOGGER_H
#include <string>
#include <SDL2/SDL.h>
#include <fstream>
#include <filesystem>
#include "string.h"

#ifndef __FUNCTION_NAME__
#ifdef WIN32 // WINDOWS
#define __FUNCTION_NAME__ __FUNCTION__
#else //*NIX
#define __FUNCTION_NAME__ __func__
#endif
#endif
#ifndef __FUNCSIG__
#define __FUNCSIG__ __PRETTY_FUNCTION__
#endif
namespace utils
{

	enum class LogLevel
	{
		error = 0,
		warn = 1,
		info = 2,
		trace = 3
	};

	const std::string get_loglevel(LogLevel pLevel);

	class Logger
	{
	public:
		/** Default constructor */
		Logger(const std::string &loggerName, LogLevel pLevel);
		Logger();
		/** Default destructor */
		virtual ~Logger();

		void init(std::filesystem::path &outputDir, LogLevel pLevel);
		void logSDLError(const std::string &msg);
		void error(const std::string &src, const std::string &msg);
		void warn(const std::string &src, const std::string &msg);
		void info(const std::string &src, const std::string &msg);
		void trace(const std::string &src, const std::string &msg);

		template <typename... Args>
		void info(const std::string &src, const std::string &msg, Args const &...args)
		{
			info(src, utils::string_format(msg, process_arg(args)...));
		}

		template <typename... Args>
		void error(const std::string &src, const std::string &msg, Args const &...args)
		{
			error(src, utils::string_format(msg, process_arg(args)...));
		}

		template <typename... Args>
		void warn(const std::string &src, const std::string &msg, Args const &...args)
		{
			warn(src, utils::string_format(msg, process_arg(args)...));
		}

		template <typename... Args>
		void trace(const std::string &src, const std::string &msg, Args const &...args)
		{
			trace(src, utils::string_format(msg, process_arg(args)...));
		}

	protected:
		void log(LogLevel pLevel, const std::string &src, const std::string &msg);
		std::ostream &getStreamWriter();

	private:
		std::string m_loggerName;
		LogLevel level;
		std::fstream out;
	};

} // namespace utils
inline utils::Logger g_sglLogger("SGL", utils::LogLevel::trace);
inline utils::Logger g_appLogger("APP", utils::LogLevel::trace);
#define VA_ARGS(...) , ##__VA_ARGS__

#define SGL_LOG_TRACE(msg, ...) g_sglLogger.trace(__FUNCSIG__, msg VA_ARGS(__VA_ARGS__));
// #define SGL_LOG_TRACE(msg) g_sglLogger.trace(__FUNCSIG__, msg);
#define SGL_LOG_ERROR(msg, ...) g_sglLogger.error(__FUNCSIG__, msg VA_ARGS(__VA_ARGS__));
#define SGL_LOG_WARN(msg) g_sglLogger.warn(__FUNCSIG__, msg);
#define SGL_LOG_INFO(msg) g_sglLogger.info(__FUNCSIG__, msg);
#define SGL_LOG_ERROR_SDL() g_sglLogger.logSDLError(__FUNCSIG__);

#define APP_LOG_TRACE(msg, ...) g_appLogger.trace(__FUNCSIG__, msg VA_ARGS(__VA_ARGS__));
#define APP_LOG_ERROR(msg) g_appLogger.error(__FUNCSIG__, msg);
#define APP_LOG_WARN(msg) g_appLogger.warn(__FUNCSIG__, msg);
#define APP_LOG_INFO(msg) g_appLogger.info(__FUNCSIG__, msg);
#define APP_LOG_ERROR_SDL() g_appLogger.logSDLError(__FUNCSIG__);

#endif // UTILS_LOGGER_H
