#pragma once

#include <string>
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

    class ConsoleLogger : public ILogger {
        protected:
            void LogImpl(LogLevel level, const std::string& message) override;
    };
}