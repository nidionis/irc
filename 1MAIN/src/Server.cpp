//
// Created by n on 16/06/25.
//

#include <Server.h>

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

struct pollfd Server::initSocket() throw(std::runtime_error) {
    struct pollfd pfd;
    if ((pfd.fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw std::runtime_error("Socket creation failed") ;
    }
    if (!setReuseAddr(pfd.fd)) {
        throw std::runtime_error("Setting socket failed");
    }
    this->_sockets[this->_nb_socket] = pfd;
    this->_nb_socket++;
    return pfd;
}

struct sockaddr_in &Server::getSockAddr(int i_socket) {
    return _addresses[i_socket];
}

struct pollfd &Server::getPfd(int i_socket) {
    return _sockets[i_socket];
}

int Server::getFd(int i_socket) {
    return _sockets[i_socket].fd;
}

static bool unBlockSocket(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        throw std::runtime_error("Failed to get socket flags");
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        throw std::runtime_error("Failed to set non-blocking mode");
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
        throw std::runtime_error("Too many sockets");
    }
    struct pollfd &pfd = _sockets[i_socket];
    struct sockaddr_in &sockAddr = _addresses[i_socket];
    configureSockAddr(sockAddr, _port);
    unBlockSocket(pfd.fd);
    if (bind(pfd.fd, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) < 0) {
        throw std::runtime_error("Bind failed");
    }
    if (listen(_sockets[i_socket].fd, MAX_CONN) < 0) {
        throw std::runtime_error("Listen failed");
    }
    std::cout << "Server listening on port " << _port << std::endl;
    return true;
}

Client *Server::waitConn(int i_socket) {
    int fd;
    int addrlen = sizeof(struct sockaddr_in);
    Client *client = new Client();
    while ((fd = accept(_sockets[i_socket].fd, (struct sockaddr *)&_addresses[i_socket], (socklen_t*)&addrlen)) < 0) {
        continue;
    }
    client->setFd(fd);
    return client;
}

Client *Server::testing_poll(int i_socket) {
    struct pollfd pfd_cli;
    struct pollfd &pfd = _sockets[i_socket];
    //int addrlen = sizeof(struct sockaddr_in);
    pfd.events = POLL_IN;
    Client *client = new Client();
    while (1) {
        int activity = ::poll(&pfd, _nb_socket, -1);
        if (activity < 0) {
            throw std::runtime_error("Poll error");
        }
        if (pfd.revents & POLLIN) {
            socklen_t addrlen = sizeof(struct sockaddr_in);
            pfd_cli.fd = ::accept(pfd.fd, \
                                (struct sockaddr *)&_addresses[i_socket], \
                                &addrlen);
            if (pfd_cli.fd < 0) {
                throw std::runtime_error("Accept failed");
            }
            printf("New client connected, socket pfd_cli is %d\n", pfd_cli.fd);
            pfd_cli.events = POLLIN;
        }
        char buffer[BUFF_SIZE + 1];
        for (int i = 1; i < MAX_SOCKET + 1; i++) {
            if (pfd_cli.fd != -1 && pfd_cli.revents & POLLIN) {
                memset(buffer, 0, BUFF_SIZE);
                ssize_t bytes_read = recv(pfd_cli.fd, buffer, BUFF_SIZE, 0);

                if (bytes_read <= 0) {
                    printf("Client disconnected, socket fd %d\n", pfd_cli.fd);
                    pfd_cli.fd = -1;
                } else {
                    printf("Received from socket %d: %s", pfd_cli.fd, buffer);
                }
            }
        }
        continue;
    }
    return client;
}

Client *Server::renameThisFunctionPlease(int i_socket) {
    while (1) {
        try {
            this->listenUp(i_socket);
        } catch (const std::runtime_error &e) {
            continue;
        }
        break;
    }
    Client *client = this->waitConn(i_socket);
    while (client->printing_loop() != QUIT) {
        continue;
    }
    //delete client;
    return NULL;
}