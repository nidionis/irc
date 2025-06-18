//
// Created by n on 16/06/25.
//

#ifndef SERVER_H
#define SERVER_H
#define MAX_SOCKET 3

#include <ircserv.hpp>

class Server {
    int             _nb_socket;
    int             _port;
    int             _sockets[MAX_SOCKET];
    std::string     _passwd;
public:
    Server();
    Server(Server &src);
    ~Server();
    Server &operator=(Server &src);

    int initSocket();
};



#endif //SERVER_H
