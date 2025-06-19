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

static bool setReuseAddr(int server_fd) {
    int reuseAddrFlag = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseAddrFlag, sizeof(reuseAddrFlag)) < 0) {
        std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
        close(server_fd);
        return false;
    }
    return true;
}

int Server::initSocket() {
    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        //thow error
        std::cerr << "Socket creation failed" << std::endl;
        return ERROR;
    }
    if (!setReuseAddr(server_fd)) {
        //thow error
        return ERROR;
    }
    this->_sockets[this->_nb_socket] = server_fd;
    this->_nb_socket++;
    return server_fd;
}

struct sockaddr_in &Server::getSockAddr(int i_socket) {
    return _addresses[i_socket];
}

int Server::getFd(int i_socket) {
    return _sockets[i_socket];
}

bool Server::unBlockSocket(int i_socket) {
    int server_socket = _sockets[i_socket];
    int flags = fcntl(server_socket, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "Failed to get socket flags" << std::endl;
        return false;
    }
    if (fcntl(server_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::cerr << "Failed to set non-blocking mode" << std::endl;
        return false;
    }
    return true;
}

static void configureSockAddr(struct sockaddr_in &sockAddr, int port) {
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = INADDR_ANY;
    sockAddr.sin_port = htons(port);
}

bool Server::listenUp(int i_socket) {
    if (i_socket >= MAX_SOCKET) {
        // throw error
        return false;
    }
    struct sockaddr_in &sockAddr = _addresses[i_socket];
    configureSockAddr(sockAddr, _port);
    if (bind(_sockets[i_socket], (struct sockaddr *)&sockAddr, sizeof(sockAddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return false;
    }
    if (listen(_sockets[i_socket], MAX_CONN) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return false;
    }
    std::cout << "Server listening on port " << _port << std::endl;
    return true;
}
