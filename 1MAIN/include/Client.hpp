/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidionis <nidionis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:02:00 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/31 17:36:15 by nidionis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include "capabilities.hpp"

# define MAX_CHANNELS 2

# define GETTER_SETTER(type, name, f_name) \
type get##f_name() const { return name; } \
void set##f_name(type value) { name = value; }

class	Channel;

class	Client
{
private:
	Server*						_server;
	std::string					_nickname;
	std::string					_username;
	std::string					_realname;
	std::vector<Channel>		_channels;
	std::vector<std::string>	_capabilities;
	std::vector<std::string>	_flags;
	bool						_must_kill;
	int							_fd_client_socket;
	struct sockaddr_in			_IPv4_client_sock_addr;
	socklen_t					_client_addrlen;

public:

	Client(void);
	Client(Server* server);
	// missing copy constructor
	~Client(void);

	GETTER_SETTER(std::string, _nickname, Nickname)
	GETTER_SETTER(std::string, _username, Username)
	GETTER_SETTER(std::string, _realname, Realname)
	GETTER_SETTER(bool, _must_kill, must_kill)
	GETTER_SETTER(int, _fd_client_socket, fd_client_socket)
	GETTER_SETTER(struct sockaddr_in, _IPv4_client_sock_addr, IPv4_client_sock_addr)
	GETTER_SETTER(socklen_t, _client_addrlen, client_addrlen)

	struct sockaddr_in&		special_getIPv4_client_sock_addr(void);	// Server_poll.cpp::pollClientConnect()
	socklen_t&				special_get_client_addrlen(void);		// Server_poll.cpp::pollClientConnect()

	bool					operator==(const Client& other) const;
	void					clientCleanup(void);

	void					newChannel(std::string& name);
	void					delChannel(Channel& channel);
	bool					hasChannel(Channel& channel);
	Channel &getChannel(void);
	ssize_t					send(std::string msg);
	std::string				getIp(void);
	ssize_t					send_banner(std::string line);
	bool					isLogged(void);

	void					setCap(const std::string& cap);
	void					resetCap(const std::string& cap);
	bool					hasCap(const std::string& cap);
	void					setFlag(const std::string& cap);
	void					resetFlag(const std::string& cap);
	bool					hasFlag(const std::string& cap);
};

#endif //CLIENT_HPP
