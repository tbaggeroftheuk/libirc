#pragma once

#include <string>
#include <memory>
#include <atomic>
#include <mutex>

#include "libirc/auth.hpp"
#include "libirc/logging.hpp"

namespace irc::server {
    class Server {
        public:
            /**
             * @brief Creates a server instance with default logging (irc::logging::ConsoleLogger)
             */
            Server();

            /**
             * @brief Creates a server instance using a logger you've specified
             */
            Server(std::shared_ptr<irc::logging::ILogger> logger);
            
            /**
             * @brief Destroys the server.
             *
             * If the server is running, it is stopped and all worker threads
             * are joined before destruction completes.
             */
            ~Server();

            /**
             * @brief Gets the last error message as a string
             * @return Returns an error message as a string of the last error
            */
            std::string GetLastError() const;

            /** 
             * @brief Bind the server to an address and port
             * @param address An Ipv4 or Ipv6 address as a string, eg 127.0.0.0
             * @param port The port to bind to
             * @return Returns false if an error (Use @ref GetLastError to see why it failed)
            */
            bool BindAddress(const std::string& address, const int port = 6667);

            /**
             * @brief Sets the authenticator.
             *
             * The provided authenticator should derive from
             * @ref irc::auth::Authenticator.
             * 
             * 
             * @param auth Shared authenticator instance. The server retains a reference for its lifetime or until a new authenticator is set.
             * See @ref auth.hpp for the interface definition.
             */
            void SetAuthenticator(std::shared_ptr<irc::auth::Authenticator> auth);

            /**
             * @brief Starts the server and blocks until it stops.
             *
             * Convenience wrapper around @ref Start() and @ref Wait().
             */
            bool Run();

            /**
             * @brief Starts the server
             * 
             * When the server has already been started it will return false.
             * 
             * @return Returns false if an error occured (Use @ref GetLastError to see why it failed) 
             */
            bool Start();

            /**
             * @brief Stops the server, kicks all clients
             * 
             * If already stopped, this returns false.
             * 
             * @param stop_message The message sent to clients before shut down. If empty nothing is sent
             * @return Returns false on failure (This is very unlikely and it's probably a double shutdown)
             */
            bool Stop(const std::string& stop_message = "");

            /**
             * @brief Waits for the server thread to exit.
             *
             * This function blocks until the server has been stopped.
             * 
             * @return Returns false when a fatal error happens
             */
            bool Wait();

            /**
             * @brief A function to check whether the server is on
             * @return Returns true if the server is running, otherwise it returns false
             */
            bool IsRunning() const;
        private:
            void SetError(const std::string& error_message);

            mutable std::mutex mErrorMessageMutex;
            std::string mLastError = "";
            std::atomic<bool> mIsRunning = false;
            class Impl;
            std::unique_ptr<Impl> mImpl;
    };
}