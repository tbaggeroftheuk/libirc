#include "libirc/message.hpp"

namespace irc {
    const std::string IRCMessage::Command() const {
        return mCommand;
    }

    const std::string IRCMessage::Prefix() const {
        return mPrefix;
    }

    const std::vector<std::string>& IRCMessage::Params() const {
        return mParameters;
    }
}