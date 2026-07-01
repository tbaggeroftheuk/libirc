#pragma once

#include <memory>
#include <string>
#include <cstddef>
#include <system_error>
#include <asio.hpp>

// Public api stuff
#include "libirc/logging.hpp"
#include "libirc/auth.hpp"

// private stuff
#include "protocol/parser.hpp"

namespace irc::server::impl {
    // Session for the client
    class ServerSession : public std::enable_shared_from_this<ServerSession> {
        public:
            ServerSession(
                asio::ip::tcp::socket socket,
                std::shared_ptr<irc::logging::ILogger> logger,
                std::shared_ptr<irc::auth::Authenticator> authenticator
            );

            void Start();
            void Stop();
        private:
            void Read();

            // TODO Impliment this
            void Send(const std::string& message) {}

            void HandleRead(std::error_code ec, std::size_t bytes);
            void HandleMessage(const std::string& message);

            asio::ip::tcp::socket mSocket;
            asio::streambuf mBuffer;

            irc::impl::protocol::IRCParser mParser;
            std::shared_ptr<irc::logging::ILogger> mLogger;
            std::shared_ptr<irc::auth::Authenticator> mAuthenticator;
    };
}