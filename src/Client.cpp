//
// Created by n on 16/06/25.
//

#include <ircserv.hpp>

Client::Client() : _nickname("default_name"), _username("default_username"), _hostname("default_hostname"), _fd(UNDEFINED_FD) {
}

Client::~Client() {}

Client::Client(Client &src) {(void)src;}

Client &Client::operator=(Client &src) {(void)src; return (src);}

int Client::acceptConn(int server_fd, struct sockaddr_in& address) {
    int addrlen = sizeof(struct sockaddr_in);
    while ((_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        continue;
    }
    return OK;
}

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
        std::cout << "Received: " << buffer << std::endl;
    }
}
