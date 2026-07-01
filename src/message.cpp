#include "libirc/message.hpp"

namespace irc {
    const std::string IRCMessage::Command() const {
        return mCommand;
    }

    const std::string IRCMessage::Prefix() const {
        return mPrefix;
    }

    const std::string& IRCMessage::Param(size_t index) const {
        return mParameters[index];
    }

    size_t IRCMessage::ParamCount() const noexcept {
        return mParameters.size();
    }
}