//
// Created by n on 16/06/25.
//

#ifndef CLIENT_H
#define CLIENT_H

#include "ircserv.hpp"
#define UNDEFINED_FD -1

class Client {
    std::string _nickname;
    std::string _username;
    std::string _hostname;
    int _fd;
public:
    Client();
    ~Client();
    Client(Client &src);
    Client &operator=(Client &src);

    int accept(int server_fd, struct sockaddr_in& address);
};

#endif //CLIENT_H
