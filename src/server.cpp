#include <asio.hpp>

#include "libirc/server.hpp"
#include "libirc/auth.hpp"

namespace irc::server {
    class Server::Impl {
        public:
            asio::io_context context;
            asio::ip::tcp::acceptor acceptor;
            
            std::shared_ptr<irc::auth::Authenticator> authenticator;

            Impl() : acceptor(context) {

            }

            void Accept() {
                auto socket = std::make_shared<asio::ip::tcp::socket>(context);

                acceptor.async_accept(*socket,
                    [this, socket](std::error_code ec) {
                        if (!ec) {
                            
                        }

                        Accept();
                });
            }
    };

    Server::Server() : mImpl(std::make_unique<Impl>()) {}
    Server::~Server() = default;

    std::string Server::GetLastError() const {
        std::lock_guard<std::mutex> lock(mErrorMessageMutex);

        return mLastError;
    }

    void Server::SetError(const std::string& error_message) {
        std::lock_guard<std::mutex> lock(mErrorMessageMutex);

        mLastError = error_message;
    }

    bool Server::BindAddress(const std::string& address, int port) {
        try {
            auto ip = asio::ip::make_address(address);

            asio::ip::tcp::endpoint endpoint(ip, port);

            mImpl->acceptor.open(endpoint.protocol());
            mImpl->acceptor.bind(endpoint);
            mImpl->acceptor.listen();

            mImpl->Accept();

            return true;
        } catch (const std::exception& e) {
            mLastError = e.what();
            return false;
        }
    }

    void Server::SetAuthenticator(std::shared_ptr<irc::auth::Authenticator> auth) {
        mImpl->authenticator = auth;
    }
}