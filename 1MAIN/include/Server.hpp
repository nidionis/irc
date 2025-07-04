/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:58:39 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/30 13:39:49 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define SERV_NAME "ircSchoolProject"
# include <Client.hpp>
# include <Channel.hpp>

//class	Channel;
//class	Client;

class	Server
{
private:
    std::string _name;
	struct sockaddr_in	IPv4_serv_sock_addr;
	int					fd_server_socket;
	char				buffer[BUFFER_SIZE];
	std::vector<Client>	vector_clients;
    std::vector<Channel> channels;

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
    void				serverCleanup(void);
	void				pollLoop(void);

	ssize_t				sendClient(Client &cli, std::string msg);
    Client              &getClient(int i);
    void	            handle(char *buffer, Client &client);
    void	            applyRequest(char *buffer, Client &client);
    void	            sendCmds(Client &client);

    Channel             &newChannel(Client &client);
};

#endif //SERVER_HPP
