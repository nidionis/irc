#include "ircserv.hpp"

/**
 * Basic testing function that sets up a socket and prints received strings
 * @param argc Argument count
 * @param argv Argument values
 * @return Exit status code
 */
int testing(int argc, char **argv)
{
    (void)argc;
    int port = std::atoi(argv[1]);
    int server_fd, client_fd;
    struct sockaddr_in address;
    char buffer[BUFF_SIZE] = {0};
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt failed" << std::endl;
        return 1;n
    }

    // Setup address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind socket to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return 1;
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;
    std::cout << "Password: " << argv[2] << std::endl;

    // Accept a connection
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Accept failed" << std::endl;
        return 1;
    }

    std::cout << "Client connected" << std::endl;

    // Basic receive and print loop
    while(1) {
        // Clear buffer
        memset(buffer, 0, sizeof(buffer));

        // Read data from client
        int bytes_read = read(client_fd, buffer, 1024);
        if (bytes_read <= 0) {
            if (bytes_read < 0)
                std::cerr << "Read failed" << std::endl;
            break;
        }

        // Print received data
        std::cout << "Received: " << buffer << std::endl;

        // Simple echo response
        send(client_fd, buffer, bytes_read, 0);
    }

    // Close sockets
    close(client_fd);
    close(server_fd);
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
