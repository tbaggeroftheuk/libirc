#include "protocol/parser.hpp"

namespace irc::impl::protocol {
    IRCMessage IRCParser::Parser(const std::string& line, bool& error_code) {
        error_code = false;

        IRCMessage message;

        std::size_t pos = 0;

        auto SkipSpaces = [&]() {
            while (pos < line.size() && line[pos] == ' ') {
                ++pos;
            }
        };

        SkipSpaces();

        // Optional prefix
        if (pos < line.size() && line[pos] == ':') {
            ++pos;

            std::size_t start = pos;

            while (pos < line.size() && line[pos] != ' ') {
                ++pos;
            }

            if (start == pos) {
                error_code = true;
                return {};
            }

            message.mPrefix = line.substr(start, pos - start);

            SkipSpaces();
        }

        // Command
        {
            std::size_t start = pos;

            while (pos < line.size() && line[pos] != ' ') {
                ++pos;
            }

            if (start == pos) {
                error_code = true;
                return {};
            }

            message.mCommand = line.substr(start, pos - start);

            SkipSpaces();
        }

        // Parameters
        while (pos < line.size()) {
            if (line[pos] == ':') {
                // Trailing parameter consumes the rest of the line
                ++pos;
                message.mParameters.emplace_back(line.substr(pos));
                break;
            }

            std::size_t start = pos;

            while (pos < line.size() && line[pos] != ' ') {
                ++pos;
            }

            if (start != pos) {
                message.mParameters.emplace_back(line.substr(start, pos - start));
            }

            SkipSpaces();
        }

        return message;
    }
}