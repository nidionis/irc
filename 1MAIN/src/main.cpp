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
        //actually configure, bind, listen, accept the connection to a socket
        // then use a printing_loop
        //but bugs : se second socket cannot bind
        client = server.renameThisFunctionPlease(0);
        delete client;
        std::cout << "-----------------------------------------------\n" ;

        server.initSocket();
        client = server.renameThisFunctionPlease(1);
        delete client;
    }
    return 0;
}
