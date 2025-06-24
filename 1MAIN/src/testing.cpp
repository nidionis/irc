//
// Created by nidionis on 16/06/25.
//

#include "ircserv.hpp"

/**
 * Initialize a server socket with reuse address option
 * @return Socket file descriptor or -1 on error
 */
int initializeServerSocket() {
    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return ERROR;
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt failed" << std::endl;
        close(server_fd);
        return ERROR;
    }
    return server_fd;
}

/**
 * Configure and bind server socket to the specified port
 * @param server_fd Server socket file descriptor
 * @param port Port number to bind
 * @param address Reference to the address structure to be configured
 * @return true on success, false on failure
 */
bool configureAndBindSocket(int server_fd, int port, struct sockaddr_in& address) {
    // Setup address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return false;
    }
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return false;
    }
    std::cout << "Server listening on port " << port << std::endl;
    return true;
}

int printing_loop(int client_fd, char *buffer) {
    memset(buffer, 0, BUFF_SIZE);
    int bytes_read = read(client_fd, buffer, BUFF_SIZE);
    if (bytes_read <= 0) {
        if (bytes_read < 0)
            std::cerr << "Read failed" << std::endl;
        return ERROR;
    }
    std::cout << "Received: " << buffer << std::endl;
    //send(client_fd, buffer, bytes_read, 0);
    return OK;
}

/**
 * Basic testing function that sets up a socket and prints received strings
 * @param argc Argument count
 * @param argv Argument values
 * @return Exit status code
 */
int testing(int argc, char **argv)
{
    (void)argc;
    int server_fd, client_fd;

    server_fd = initializeServerSocket();
    if (server_fd < 0) {
        return ERROR;
    }

    int port = std::atoi(argv[1]);
    struct sockaddr_in address;
    if (!configureAndBindSocket(server_fd, port, address)) {
        close(server_fd);
        return ERROR;
    }

    //std::cout << "Password: " << argv[2] << std::endl;

    int addrlen = sizeof(address);
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Accept failed" << std::endl;
        return 1;
    }

    std::cout << "Client connected" << std::endl;

    char buffer[BUFF_SIZE] = {0};
    while(1) {
        if (printing_loop(client_fd, buffer) == ERROR) {
            break ;
        }
    }

    // Close sockets
    close(client_fd);
    close(server_fd);
    return 0;
}
