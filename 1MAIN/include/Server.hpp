/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:58:39 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/30 12:36:26 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

class	Client;

class	Server
{
private:
	struct sockaddr_in	IPv4_serv_sock_addr;
	int					fd_server_socket;
	char				buffer[BUFFER_SIZE];
	std::vector<Client>	vector_clients;

	void				initServerSocket(void);
	void				setServerSockopt(void);
	void				bindServerSocket(void);
	void				listenServerSocket(void);

	void				pollFailHandler(poll_data* poll_data);
	void				pollClientHandler(poll_data* poll_data);
	void				pollClientConnect(poll_data* poll_data);
	void				pollClientRecv(poll_data* poll_data);
	void				pollClientDisconnect(poll_data* poll_data);

public:
	Server(void);
	~Server(void);

	void				serverSetup(void);
	void				pollLoop(void);
	void				serverCleanup(void);
};

	//int					_nb_socket;
	//int					_port;
	//struct pollfd			_sockets[MAX_SOCKET];
	//struct sockaddr_in	_addresses[MAX_SOCKET];
	//std::string			_passwd;

	//std::vector<Client>	list_clients;

	/*int				getFd(int i_socket);
	struct pollfd&		getPfd(int i_socket);
	struct pollfd		initSocket() throw(std::runtime_error);
	struct sockaddr_in&	getSockAddr(int i_socket);
	bool				listenUp(int i_socket);
	Client*				connection(int i_socket);
	void				pollRun();
	int					receivingSocket();*/

#endif //SERVER_HPP
