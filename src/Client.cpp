//
// Created by n on 16/06/25.
//

#include <ircserv.hpp>

Client::Client() : _nickname("default_name"), _username("default_username"), _hostname("default_hostname"), _fd(UNDEFINED_FD) {
}

Client::~Client() {}

Client::Client(Client &src) {(void)src;}

Client &Client::operator=(Client &src) {(void)src; return (src);}

int Client::accept(int server_fd, struct sockaddr_in& address) {
    int addrlen = sizeof(struct sockaddr_in);
    if (_fd == UNDEFINED_FD) {
        // trow error
        std::cerr << "Accept failed" << std::endl;
        return ERROR;
    }
    if ((_fd = ::accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        // trow error
        std::cerr << "Accept failed" << std::endl;
        return ERROR;
    }
    return OK;
}
