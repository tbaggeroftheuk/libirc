#include <iostream>

#include "libirc/server.hpp"

int main() {
    irc::server::Server server;

    if (!server.BindAddress("127.0.0.1", 6667, true)) {
        std::cerr << "Failed to bind to address, reason: " << server.GetLastError();
        return 0;
    }

    server.Run();
    server.Stop();
    return 0;
}