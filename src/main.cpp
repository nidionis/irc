#include "ircserv.hpp"

/**
 * The testing function - currently empty implementation
 * @param argc Argument count
 * @param argv Argument values
 * @return Exit status code
 */
int testing(int argc, char **argv)
{
    // Empty for now
    (void)argc;
    (void)argv;
    return 0;
}

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
    return testing(argc, argv);
}
