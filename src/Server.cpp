//
// Created by n on 16/06/25.
//

#include "Server.h"

Server::Server() : _nb_socket(0), _port(6667), _passwd("<PASSWORD>") {}

Server::~Server() {}

Server::Server(Server &src) {
    (void)src;
    (void)_port;
}

Server &Server::operator=(Server &src) {
    (void)src;
    return *this;
}

int Server::initSocket() {
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
    this->_sockets[this->_nb_socket] = server_fd;
    this->_nb_socket++;
    return server_fd;
}
