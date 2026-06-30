#include <utility>
#include <system_error>
#include <asio.hpp>

#include "server/server_session.hpp"

namespace irc::server::impl {
    ServerSession::ServerSession(
        asio::ip::tcp::socket socket,
        std::shared_ptr<irc::logging::ILogger> logger,
        std::shared_ptr<irc::auth::Authenticator> authenticator
    ) : mSocket(std::move(socket)), mLogger(logger), mAuthenticator(authenticator) {}

    void ServerSession::Start() {
        Read();
    }

    void ServerSession::Stop() {
        std::error_code ec;

        mSocket.shutdown(
            asio::ip::tcp::socket::shutdown_both,
            ec
        );

        mSocket.close(ec);
    }

    void ServerSession::Read() {
        auto self = shared_from_this();

        asio::async_read_until(
            mSocket,
            mBuffer,
            "\r\n",
            [this, self](std::error_code ec, std::size_t bytes) {
                HandleRead(ec, bytes);
            }
        );
    }

    void ServerSession::HandleRead(
        std::error_code ec,
        std::size_t
    ) {
        if (ec) {
            Stop();
            return;
        }

        std::istream stream(&mBuffer);

        std::string message;
        std::getline(stream, message);

        HandleMessage(message);

        Read();
    }

    void ServerSession::HandleMessage(
        const std::string& message
    ) {
        // TODO have something like below
        // irc::IRCMessage msg = irc::parser::Parse(message);
        //
        // switch(msg.Command()) {
        //     case Command::Nick:
        // }

        std::string copy = message;

        mLogger->Log(
            irc::logging::LogLevel::Info,
            "Received: {}",
            copy
        );
    }
}