/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:58:51 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/30 13:40:03 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <main.hpp>

class Client
{
private:
    Server              	*server;
    std::string         	_nickname;
    std::string         	_username;
    std::string         	_realname;
    std::string         	_hostname;
    std::vector<Channel>    channels;

public:

	struct sockaddr_in	IPv4_client_sock_addr;
	int					fd_client_socket;
	socklen_t			client_addrlen;

	Client(void);
    Client(Server *server);
	~Client(void);
    bool operator==(const Client &other) const;

    void				setNickname(std::string buffer);
    std::string         &getNickname(void);
    void            	setUsername(std::string buffer);
    std::string         &getUsername(void);
    void                setRealname(std::string buffer);
    std::string         &getRealname(void);
    void                setHostname(std::string buffer);
    std::string         &getHostname(void);
    void				clientCleanup(void);

    Channel             *newChannel(std::string &name);
    void                *delChannel(Channel &channel);
    ssize_t             send(std::string msg);
};

#endif //CLIENT_HPP
