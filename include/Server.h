//
// Created by n on 16/06/25.
//

#ifndef SERVER_H
#define SERVER_H
#define MAX_SOCKET 3
#define MAX_CONN 3
#define UNDEFINED -1

#include <ircserv.hpp>

class Server {
    int                         _nb_socket;
    int                         _port;
    int                         _sockets[MAX_SOCKET];
    struct sockaddr_in   _addresses[MAX_SOCKET];
    std::string                 _passwd;
public:
    Server();
    Server(Server &src);
    ~Server();
    Server &operator=(Server &src);

    int initSocket();
    struct sockaddr_in &getSockAddr(int i_socket);
    int getFd(int i_socket);
    bool unBlockSocket(int i_socket);
    bool listenUp(int i_socket);

    //testing purpose only
    int printing_loop(int i_socket);
};

#endif //SERVER_H
