//
// Created by n on 16/06/25.
//

#include <ircserv.hpp>

Client::Client() : _nickname("default_name"), _username("default_username"), _hostname("default_hostname"), _fd(UNDEFINED_FD) {
}

Client::~Client() {}

Client::Client(Client &src) {(void)src;}

Client &Client::operator=(const Client &src) {
    if (this == &src)
        return *this;
    _nickname = src._nickname;
    _username = src._username;
    _hostname = src._hostname;
    _fd = src._fd;
    // Add any other fields that need copying here
    return *this;
}

void Client::setFd(int fd) {
    _fd = fd;
}

//int Client::acceptConn(int server_fd, struct sockaddr_in& address) {
//    int addrlen = sizeof(struct sockaddr_in);
//    while ((_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
//        continue;
//    }
//    return OK;
//}

int Client::printing_loop() {
    unsigned char buffer[BUFF_SIZE];
    int bytes_read;
    while (1) {
        bytes_read = read(_fd, buffer, BUFF_SIZE - 1);
        if (bytes_read <= 0) {
            if (bytes_read < 0) {
                std::cerr << "Read failed: "
                          << strerror(errno)  // Print system error description
                          << " (Error code: " << errno << ")"
                          << std::endl;
                return ERROR;
            } else {
                std::cerr << "Connection closed by peer" << std::endl;
                return QUIT;
            }
        }
        buffer[bytes_read] = '\0';
        std::cout << "Received: " << buffer;
    }
}
