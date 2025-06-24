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
    Client *client;
    while (1)
    {
        client = server.testing_poll(0);
        delete client;
    }
    return 0;
}
