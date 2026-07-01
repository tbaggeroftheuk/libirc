#pragma once

#include <string>
#include <vector>

namespace irc {
    class IRCMessage {
        public:
            const std::string Command() const;
            const std::string Prefix() const;
            const std::vector<std::string>& Params() const;
        private:
            std::string mCommand;
            std::string mPrefix;
            std::vector<std::string> mParameters;
    };
}