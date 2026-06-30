#pragma once

#include <string>
#include <mutex>
#include <utility>
#include <fmt/format.h>

namespace irc::logging {
    enum class LogLevel {
        Info,
        Warning,
        Error,
        Fatal
    };

    /**
     * @brief A utility function to take a LogLevel and return a string of the LogLevel
     * 
     * @return Returns a std::string
     */
    std::string LogLevel2String(LogLevel level);
    
    /**
     * @brief An interface class used by the server class and client class to log
     * 
     * This must be thread safe!
     * 
     * To impliment a logger, inherit from this class and impliment LogImpl
     */
    class ILogger {
        public:
            virtual ~ILogger() = default;

            template<typename... Args>
            void Log(LogLevel level, fmt::format_string<Args...> format, Args&&... args) {
                LogImpl(level, fmt::format(format, std::forward<Args>(args)...));
            }

        protected:
            virtual void LogImpl(LogLevel level, const std::string& message) = 0;
    };

    /**
     * @brief Logs to the console, uses a mutex for thread safe logging
     */
    class ConsoleLogger : public ILogger {
        protected:
            void LogImpl(LogLevel level, const std::string& message) override;
            static std::mutex mMutex;    
    };

    /**
     * @brief A logger implimentation that discards all messages
     */
    class NullLogger : public ILogger {
        protected:
            void LogImpl(LogLevel, const std::string&) override {}
    };
}