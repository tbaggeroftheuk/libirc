#include <iostream>
#include <chrono>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <string>

#include "libirc/logging.hpp"


std::string GetTimestamp() {
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t now_time = system_clock::to_time_t(now);

    std::tm tm{};

    // Due to the differences between windows/posix the function signatures are reversed....
    #ifdef _WIN32
        localtime_s(&tm, &now_time);
    #else
        localtime_r(&now_time, &tm);
    #endif

    std::ostringstream ss;
    ss << std::put_time(&tm, "%H:%M:%S");
    return ss.str();
}

namespace irc::logging {
    std::mutex ConsoleLogger::mMutex;

    void ConsoleLogger::LogImpl(LogLevel level, const std::string& message) {
        std::ostringstream ss;
        ss << "[" << GetTimestamp() << "] " << "[" << LogLevel2String(level) << "] " << message << '\n';

        std::lock_guard<std::mutex> lock(mMutex);

        if (level == LogLevel::Error || level == LogLevel::Fatal) {
            std::cerr << ss.str();
        } else {
            std::cout << ss.str();
        }
    }
}