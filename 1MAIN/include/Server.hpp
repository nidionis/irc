/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:29:47 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/10 14:40:50 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>

# include <cerrno>
# include <csignal>
# include <cstring>

# include <fcntl.h>		// fcntl()
# include <ifaddrs.h>	// getifaddrs()
# include <signal.h>
# include <stdio.h>		// perror()

# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/types.h>

# include "main.hpp"
# include "Channel.hpp"

# define SERV_NAME			"ircSchoolProject"
# define BUFFER_SIZE		1024
# define QUEUE_SIZE			16
# define MAX_CLIENTS		3
# define MAX_CONNECTIONS	(MAX_CLIENTS + 1)
# define BUFFER_SIZE		1024

class	Client;
class	Channel;

struct	poll_data;

class	Server
{
private:
	std::string				_name;
	std::string				_passwd;
	struct sockaddr_in		_IPv4_serv_sock_addr;
	int						_fd_server_socket;
	char					_buffer[BUFFER_SIZE];
	std::vector<Client>		_vector_clients;
	std::vector<Channel>	_channels;

	void	initServerSocket(void);								// Server.cpp
	void	setServerSockopt(void);								// Server.cpp
	void	bindServerSocket(void);								// Server.cpp
	void	listenServerSocket(void);							// Server.cpp

	void	pollFailHandler(poll_data* poll_data);				// Server_poll.cpp
	void	pollClientHandler(poll_data* poll_data);			// Server_poll.cpp
	void	pollClientConnect(poll_data* poll_data);			// Server_poll.cpp
	void	pollClientRecv(poll_data* poll_data);				// Server_poll.cpp
	void	pollClientDisconnect(poll_data* poll_data);			// Server_poll.cpp

public:
	Server(void);												// Server.cpp
	// missing arg list constructor ?							// Server.cpp
	// missing copy constructor									// Server.cpp
	~Server(void);												// Server.cpp

	std::string&	getName(void);								// Server.cpp
	std::string		getIp(void);								// Server.cpp
	in_port_t		getPort(void);								// Server.cpp
	void			server_init(int port, std::string passwd);	// Server.cpp
	void			serverSetup(void);							// Server.cpp
	void			serverCleanup(void);						// Server.cpp
	bool			checkPasswd(std::string passwd);			// Server.cpp

	void			pollLoop(void);								// Server_poll.cpp

	ssize_t			sendClient(Client& cli, std::string msg);	// Server_Client.cpp
	Client&			getClient(int i);							// Server_Client.cpp
	Client&			getClient(const std::string& nick);			// Server_Client.cpp
	void			handleClient(char* buffer, Client& client);	// Server_Client.cpp
	bool			clientHasNick(std::string const& nick);		// Server_Client.cpp
	bool			clientHasUser(std::string const& user);		// Server_Client.cpp
	
	bool			hasChannel(std::string const& channel);		// Server_Channel.cpp
	void			pushChannel(Channel& channel);				// Server_Channel.cpp
	void			delChannel(Channel& channel);				// Server_Channel.cpp
	Channel&		getChannel(std::string const& channel);		// Server_Channel.cpp
};

extern volatile sig_atomic_t	exit_program;	// Server_poll.cpp

std::string			getLocalIPv4Address(void);	// Server.cpp
void				handle_signal(int sig);		// Server_poll.cpp

#endif //SERVER_HPP
