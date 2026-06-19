#pragma once

#include <string>

namespace irc::auth {
    class Authenticator {
        public:
            virtual ~Authenticator() = default;

            virtual bool OnConnect(const std::string& ip) {
                return true;
            }

            // Called for the PASS command
            virtual bool AuthenticatePassword(
                const std::string& password) {
                return true;
            }

            // Called for NICK/USER registration
            virtual bool AuthenticateUser(
                const std::string& username) {
                return true;
            }

            // Called after full registration
            virtual void OnLogin(
                const std::string& username) {}
                
            virtual void OnDisconnect(
                const std::string& username) {}
    };
}