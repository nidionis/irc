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

#define GETTER_SETTER(type, name, f_name) \
	type get##f_name() { return name; } \
	void set##f_name(type value) { name = value; }

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
	GETTER_SETTER(std::string, _nickname, Nickname)
	GETTER_SETTER(std::string, _username, Username)
	GETTER_SETTER(std::string, _realname, Realname)
	GETTER_SETTER(std::string, _hostname, Hostname)

	struct sockaddr_in	IPv4_client_sock_addr;
	int					fd_client_socket;
	socklen_t			client_addrlen;

	Client(void);
    Client(Server *server);
	~Client(void);
    bool operator==(const Client &other) const;
	void clientCleanup(void);

    Channel             *newChannel(std::string &name);
    void                *delChannel(Channel &channel);
    ssize_t             send(std::string msg);
};

#endif //CLIENT_HPP
