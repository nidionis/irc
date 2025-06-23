#include "ircserv.hpp"

/**
 * Main entry point for the IRC server
 * @param argc Argument count
 * @param argv Argument values
 * @return Exit status code
 */
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }
    close(0);
    std::cout << "Starting IRC server..." << std::endl;

    Server server;
    server.initSocket();
    server.listenUp(0);
    Client *client = server.acceptConn(0);
    while (client->printing_loop() != QUIT) {
        continue ;
    }
//    char ch;
//    while (std::cin.get(ch)) {
//        if (ch == '\r' || ch == '\n') {
//            // Handle command...
//
//            // Show prompt after carriage return
//            std::cout << "> " << std::flush;
//        }
//    }
    return 0;
}
