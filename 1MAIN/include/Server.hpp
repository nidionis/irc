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
#include <string>

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

    std::string         &getName();
	void				serverSetup(void);
    void				serverCleanup(void);
	void				pollLoop(void);

	ssize_t				sendClient(Client &cli, std::string msg);
    Client              &getClient(int i);
    void	            handle(char *buffer, Client &client);
    void	            applyRequest(char *buffer, Client &client);
    //void	            sendCmds(Client &client);

	bool				hasNick(std::string const &nick);
	bool				hasUser(std::string const &user);
	bool				hasChannel(std::string const &channel);

    void                pushChannel(Channel &channel);
    void                delChannel(Channel &channel);
	Channel             &getChannel(std::string const &channel);
};

#endif //SERVER_HPP
