//
// Created by n on 16/06/25.
//

#include "Server.h"

Server::Server() : _nb_socket(0), _port(6667), _passwd("<PASSWORD>") {}

Server::~Server() throw() {}

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
        return false;
    }
    return true;
}

int Server::initSocket() throw(std::exception) {
    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw std::exception("Socket creation failed") ;
    }
    if (!setReuseAddr(server_fd)) {
        throw("Setting socket failed");
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

static bool unBlockSocket(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        throw("Failed to get socket flags");
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        throw("Failed to set non-blocking mode");
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
        throw("Too many sockets");
    }
    struct sockaddr_in &sockAddr = _addresses[i_socket];
    configureSockAddr(sockAddr, _port);
    unBlockSocket(_sockets[i_socket]);
    if (bind(_sockets[i_socket], (struct sockaddr *)&sockAddr, sizeof(sockAddr)) < 0) {
        throw("Bind failed");
    }
    if (listen(_sockets[i_socket], MAX_CONN) < 0) {
        throw("Listen failed");
    }
    std::cout << "Server listening on port " << _port << std::endl;
    return true;
}

Client *Server::acceptConn(int i_socket) {
    int fd;
    int addrlen = sizeof(struct sockaddr_in);
    Client *client = new Client();
    while ((fd = accept(_sockets[i_socket], (struct sockaddr *)&_addresses[i_socket], (socklen_t*)&addrlen)) < 0) {
        continue;
    }
    client->setFd(fd);
    return client;
}
