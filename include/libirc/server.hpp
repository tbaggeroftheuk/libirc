#pragma once

#include <string>
#include <memory>

#include "libirc/auth.hpp"

namespace irc::server {
    class Server {
        public:
            Server();
            ~Server();

            /*
            * @brief Gets the last error message as a string
            * @return Returns an error message as a string of the last error
            */
            std::string GetLastError();

            /*
            * @brief Bind the server to an address and port
            * @param address An Ipv4 or Ipv6 address as a string, eg 127.0.0.0
            * @param port The port to bind to
            * @return Returns false if an error (Use GetLastError to see why it failed)
            */
            bool BindAddress(const std::string& address, const int port = 6667);

            void SetAuthenticator(std::shared_ptr<irc::auth::Authenticator> auth);
        private:
            std::string mLastError = "";

            class Impl;
            std::unique_ptr<Impl> mImpl;
    };
}