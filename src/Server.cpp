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
        //thow error
        std::cerr << "Socket creation failed" << std::endl;
        return ERROR;
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        //throw error
        std::cerr << "setsockopt failed" << std::endl;
        close(server_fd);
        return ERROR;
    }
    this->_sockets[this->_nb_socket] = server_fd;
    this->_nb_socket++;
    return server_fd;
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

bool Server::configureAndBindSocket(int i_socket) {
    struct sockaddr_in &address = _addresses[i_socket];
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);

    if (i_socket == UNDEFINED && _nb_socket > 0) {
        i_socket = _nb_socket - 1;
    } else if (i_socket >= MAX_SOCKET) {
        //thow error
        return false;
    }
    if (bind(_sockets[i_socket], (struct sockaddr *)&address, sizeof(address)) < 0) {
        //thow error
        std::cerr << "Bind failed" << std::endl;
        return false;
    }

    if (listen(_sockets[i_socket], MAX_CONN) < 0) {
        //thow error
        std::cerr << "Listen failed" << std::endl;
        return false;
    }

    std::cout << "Server listening on port " << _port << std::endl;
    return true;
}

int Server::printing_loop(int i_socket) {
    unsigned char buffer[BUFF_SIZE];
    int bytes_read;

    while (1) {
        bytes_read = read(_sockets[i_socket], buffer, BUFF_SIZE - 1);
        if (bytes_read <= 0) {
            // Add detailed error reporting
            if (bytes_read < 0) {
                //std::cerr << "Read failed: "
                //          << strerror(errno)  // Print system error description
                //          << " (Error code: " << errno << ")"
                //          << std::endl;

                //// Handle specific error scenarios
                //switch(errno) {
                //    case EAGAIN:    // Non-blocking socket, no data
                //        break ;
                //    //case EWOULDBLOCK:
                //    //    std::cerr << "No data available (non-blocking socket)" << std::endl;
                //    //    break;
                //    case EBADF:     // Invalid file descriptor
                //        std::cerr << "Invalid socket descriptor" << std::endl;
                //        break;
                //    case EINTR:     // Interrupted system call
                //        std::cerr << "Read interrupted" << std::endl;
                //        continue;   // Try reading again
                //    default:
                //        break;
                //}

                //return ERROR;
            } else {
                std::cerr << "Connection closed by peer" << std::endl;
                return OK;  // Graceful connection closure
            }
        }
        sleep(2);

        // Null-terminate buffer for safe string handling
        buffer[bytes_read] = '\0';

        // Optional: process received data
         std::cout << "Received: " << buffer << std::endl;
    }
}

//int Server::printing_loop(int i_socket) {
//
//    unsigned char buffer[BUFF_SIZE];
//    int bytes_read;
//
//    while (1) {
//        bytes_read = read(_sockets[i_socket], buffer, BUFF_SIZE - 1);
//        if (bytes_read <= 0) {
//            if (bytes_read < 0) {
//                std::cerr << "Read failed" << std::endl;
//                return ERROR;
//            } else {
//                std::cerr << "toto" << std::endl;
//            }
//        }
//    }
//    std::cout << "Received: " << buffer << std::endl;
//    return OK;
//}
