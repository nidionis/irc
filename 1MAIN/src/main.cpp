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
    server.pollRun();
    while (1) {
        int socket_receiving  = server.receivingSocket();
        std::cout << socket_receiving << std::endl;
        sleep(1);
    }
    return 0;
}
