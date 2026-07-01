#pragma once

#include <string>

#include "libirc/message.hpp"

namespace irc::impl::protocol {
    class IRCParser {
        public:
            IRCMessage Parser(const std::string& line, bool& error_code);
    };
}