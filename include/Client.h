//
// Created by n on 16/06/25.
//

#ifndef CLIENT_H
#define CLIENT_H

#include "ircserv.hpp"
#define UNDEFINED_FD -1
#define QUIT -2

class Client {
    std::string _nickname;
    std::string _username;
    std::string _hostname;
    int _fd;
public:
    Client();
    ~Client();
    Client(Client &src);
    Client &operator=(const Client &src);

    void    setFd(int fd);
    // debug purpose only
    int printing_loop();
};

#endif //CLIENT_H
