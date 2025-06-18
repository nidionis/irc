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
    (void)server;
    sleep(20);
    //testing(argc, argv);
    return 0;
}
