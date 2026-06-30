#include <thread>
#include <utility>
#include <memory>

#include <asio.hpp>

#include "libirc/server.hpp"
#include "libirc/auth.hpp"
#include "libirc/logging.hpp"

#include "server/server_session.hpp"

namespace irc::server {
    class Server::Impl {
        public:
            asio::io_context context;
            asio::ip::tcp::acceptor acceptor;
            asio::executor_work_guard<asio::io_context::executor_type> work;

            std::shared_ptr<irc::auth::Authenticator> authenticator;
            std::shared_ptr<irc::logging::ILogger> mLogger;

            std::thread mThread;

            Impl() 
                : acceptor(context),
                work(asio::make_work_guard(context)) {
            }

            void Start() {
                Accept();

                mThread = std::thread([this]() {
                    context.run();
                });
            }

            void Stop() {
                work.reset();
                context.stop();
            }

            void Accept() {
                auto socket = std::make_shared<asio::ip::tcp::socket>(context);

                acceptor.async_accept(*socket,
                    [this, socket](std::error_code ec) {
                        if (!ec) {
                            auto session = std::make_shared<irc::server::impl::ServerSession>(
                                std::move(*socket),
                                mLogger,
                                authenticator
                            );

                            session->Start();
                        }

                        Accept();
                });
            }
    };

    Server::Server() : mImpl(std::make_unique<Impl>()) {
        mImpl->mLogger = std::make_shared<irc::logging::ConsoleLogger>();
    }

    Server::Server(std::shared_ptr<irc::logging::ILogger> logger) : mImpl(std::make_unique<Impl>()) {
        mImpl->mLogger = std::move(logger);
    }

    Server::~Server() = default;

    std::string Server::GetLastError() const {
        std::lock_guard<std::mutex> lock(mErrorMessageMutex);

        return mLastError;
    }

    void Server::SetError(const std::string& error_message) {
        std::lock_guard<std::mutex> lock(mErrorMessageMutex);

        mLastError = error_message;
    }

    bool Server::IsRunning() const {
        return mIsRunning.load();
    }

    bool Server::BindAddress(const std::string& address, int port) {
        try {
            auto ip = asio::ip::make_address(address);

            asio::ip::tcp::endpoint endpoint(ip, port);

            mImpl->acceptor.open(endpoint.protocol());
            mImpl->acceptor.bind(endpoint);
            mImpl->acceptor.listen();

            mImpl->mLogger->Log(irc::logging::LogLevel::Info, "[Server] Bound to address: {}:{}", address, port);
            return true;
        } catch (const std::exception& e) {
            mImpl->mLogger->Log(irc::logging::LogLevel::Error, "[Server] Failed to bind to address: {}:{}, Call GetLasteError for more infomation", address, port);
            SetError(e.what());
            return false;
        }
    }

    bool Server::Start() {
        // If already running, reject and return false
        if (mIsRunning.load()) {
            mImpl->mLogger->Log(irc::logging::LogLevel::Error, "[Server] Server is already running!");
            SetError("Error: server is already running");
            return false;
        }

        // Start the server
        mImpl->Start();

        mIsRunning.store(true);

        return true;
    }

    bool Server::Wait() {
        if (mImpl->mThread.joinable()) {
            mImpl->mThread.join();
        }

        return true;
    }

    bool Server::Stop(const std::string& stop_message) {
        // If not running, reject and return false
        if (!mIsRunning.load()) {
            mImpl->mLogger->Log(irc::logging::LogLevel::Error, "[Server] Failed to stop server: Server is not running");
            SetError("Failed to stop server: server is already running");
            return false;
        }
    
        // If the stop message isn't empty broadcast the message
        if (!stop_message.empty()) {
            // TODO Broadcast the shutdown message here
        }

        // Stop the server listening
        mImpl->Stop();

        mIsRunning.store(false);

        return true;
    }

    bool Server::Run() {
        if (!Start()) {
            return false;
        }

        if(!Wait()) {
            return false;
        }

        return true;
    }

    void Server::SetAuthenticator(std::shared_ptr<irc::auth::Authenticator> auth) {
        mImpl->authenticator = std::move(auth);
    }
}