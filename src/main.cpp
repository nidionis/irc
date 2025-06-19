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
    std::cout << "Starting IRC server..." << std::endl;

    Server server;
    Client client;
    server.initSocket();
    server.listenUp(0);
    client.acceptConn(server.getFd(0), server.getSockAddr(0));
    while (client.printing_loop() != QUIT) {
        continue ;
    }
    return 0;
}
