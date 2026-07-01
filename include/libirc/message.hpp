#pragma once

#include <string>
#include <vector>
#include <optional>

namespace irc::impl::protocol {
    class IRCParser;
}

namespace irc {
    class IRCMessage {
        friend class irc::impl::protocol::IRCParser;

        public:
            const std::string Command() const;
            const std::string Prefix() const;
            
            size_t ParamCount() const noexcept;

            const std::string& Param(size_t index) const;
        private:
            std::string mCommand;
            std::string mPrefix;
            std::vector<std::string> mParameters;
    };
}