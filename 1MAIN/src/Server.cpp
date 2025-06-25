//
// Created by n on 16/06/25.
//

#include <Server.h>

Server::Server() : _nb_socket(0), _port(6667), _passwd("<PASSWORD>") {}

Server::~Server() throw() {}

//Server::Server(Server &src) {
//    (void)src;
//    (void)_port;
//}
//
//Server &Server::operator=(Server &src) {
//    (void)src;
//    return *this;
//}

static void configureSockAddr(struct sockaddr_in &sockAddr, int port);
static bool unBlockSocket(int fd);
static bool ft_setsockopt(int server_fd);

struct pollfd Server::initSocket() throw(std::runtime_error) {
    struct pollfd pfd;
    while ((pfd.fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == 0) {
        continue ;
    }
    if (pfd.fd < 0) {
        throw std::runtime_error("Socket creation failed");
    }
    if (!ft_setsockopt(pfd.fd)) {
        throw std::runtime_error("Setting socket failed");
    }
    pfd.events = POLLIN | POLLHUP;
    this->_sockets[this->_nb_socket] = pfd;
    this->_nb_socket++;
    return pfd;
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
    if (listen(pfd.fd, MAX_SOCKET) < 0) {
        throw std::runtime_error("Listen failed");
    }
    std::cout << "Server listening on port " << _port << std::endl;
    return true;
}

Client *Server::connection(int i_socket) {
    int fd;
    int addrlen = sizeof(struct sockaddr_in);
    Client *client = new Client();
    while ((fd = accept(_sockets[i_socket].fd, (struct sockaddr *)&_addresses[i_socket], (socklen_t*)&addrlen)) < 0) {
        continue;
    }
    //error handling not implemented
    std::cout << "Client connected." << std::endl;
    client->setFd(fd);
    return client;
}

void Server::pollRun() {
    int activity;
    activity = ::poll(_sockets, _nb_socket, -1);
    if (activity < 0) {
        throw std::runtime_error("Poll error");
    }
}

int Server::receivingSocket() {
    //printf("nb_socket = %d\n", _nb_socket);
    for (int j = 0; j < _nb_socket; j++) {
        printf("revents = %d\n", _sockets[j].revents);
        if (_sockets[j].revents & POLLIN) {
            return j;
        }
    }
    throw std::runtime_error("Nothing to receive");
}

//Client *Server::listenAndConn(int i_socket) {
//    while (1) {
//        try {
//            this->listenUp(i_socket);
//        } catch (const std::runtime_error &e) {
//            continue;
//        }
//        break;
//    }
//    while (1)
//    {
//        Client *client = this->connection(i_socket);
//        while (client->printing_loop() != QUIT) {
//            continue;
//        }
//    }
//    return NULL;
//}

/*
 *getter and setters
 */
struct sockaddr_in &Server::getSockAddr(int i_socket) {
    return _addresses[i_socket];
}

struct pollfd &Server::getPfd(int i_socket) {
    return _sockets[i_socket];
}

int Server::getFd(int i_socket) {
    return _sockets[i_socket].fd;
}

/* static functions
 * used to configure socket options
 * or set variables
 */
static bool ft_setsockopt(int server_fd) {
    int reuseAddrFlag = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseAddrFlag, sizeof(reuseAddrFlag)) < 0) {
        return false;
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_KEEPALIVE, &reuseAddrFlag, sizeof(reuseAddrFlag)) < 0) {
        return false;
    }
    return true;
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
