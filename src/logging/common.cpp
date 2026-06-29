#include "libirc/logging.hpp"

namespace irc::logging {
    std::string LogLevel2String(LogLevel level) {
        switch (level) {
            case (LogLevel::Info):
                return "Info";
                break;
            case (LogLevel::Warning):
                return "Warning";
                break;
            case (LogLevel::Error):
                return "Error";
                break;
            case (LogLevel::Fatal):
                return "Fatal";
                break;
            default:
                return "Unknown"; // Techincally should never happen unless a new member is added to the enum
                break;
        }
    }
}