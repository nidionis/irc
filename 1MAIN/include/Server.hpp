/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:58:39 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/07 12:46:21 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>

# include <cerrno>
# include <csignal>
# include <cstring>

# include <fcntl.h>   // fcntl()
# include <ifaddrs.h> // getifaddrs()
# include <signal.h>
# include <stdio.h>   // perror()

# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/types.h>

# include "main.hpp"
# include "Channel.hpp"

# define SERV_NAME "ircSchoolProject"
# define BUFFER_SIZE 1024
# define QUEUE_SIZE 16

# define MAX_CLIENTS 3
# define MAX_CONNECTIONS (MAX_CLIENTS + 1)
# define BUFFER_SIZE 1024

class Client;
class Channel;

class Server
{
private:
    std::string _name;
    std::string _passwd;
    struct sockaddr_in IPv4_serv_sock_addr;
    int fd_server_socket;
    char buffer[BUFFER_SIZE];
    std::vector<Client> vector_clients;
    std::vector<Channel> channels;

    void initServerSocket(void);
    void setServerSockopt(void);
    void bindServerSocket(void);
    void listenServerSocket(void);

    void pollFailHandler(poll_data* poll_data);
    void pollClientHandler(poll_data* poll_data);
    void pollClientConnect(poll_data* poll_data);
    void pollClientRecv(poll_data* poll_data);
    void pollClientDisconnect(poll_data* poll_data);

public:
    Server(void);
    ~Server(void);

    void	server_init(int port, std::string passwd);
    std::string& getName();
    void serverSetup();
    void serverCleanup(void);
    void pollLoop(void);

    ssize_t	sendClient(Client& cli, std::string msg);
    Client& getClient(int i);
    Client& getClient(const std::string& nick);
    std::string getIp(void);
    in_port_t getPort();
    void        handle(char* buffer, Client& client);

    bool hasNick(std::string const& nick);
    bool hasUser(std::string const& user);
    bool hasChannel(std::string const& channel);

    void pushChannel(Channel& channel);
    void delChannel(Channel& channel);
    Channel& getChannel(std::string const& channel);

    bool	checkPasswd(std::string passwd);
};

void handle_signal(int sig);
void processCommand(Server &server, Client &client, std::string input);
std::string	getLocalIPv4Address(void);

#endif //SERVER_HPP
