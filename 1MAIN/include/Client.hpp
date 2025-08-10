/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:02:00 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/10 13:44:54 by lahlsweh         ###   ########.fr       */
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
	Server						*_server;
	std::string					_nickname;
	std::string					_username;
	std::string					_realname;
	std::vector<Channel>		_channels;
	std::vector<std::string>	_capabilities;
	std::vector<std::string>	_flags;
	bool						_must_kill;

public:

	struct sockaddr_in			IPv4_client_sock_addr;
	int							fd_client_socket;
	socklen_t					client_addrlen;

	Client(void);
	Client(Server *server);
	// missing copy constructor
	~Client(void);

	GETTER_SETTER(std::string, _nickname, Nickname)
	GETTER_SETTER(std::string, _username, Username)
	GETTER_SETTER(std::string, _realname, Realname)
	GETTER_SETTER(bool, _must_kill, must_kill)

	bool		operator==(const Client &other) const;
	void		clientCleanup(void);

	Channel*	newChannel(std::string& name);
	void		delChannel(Channel& channel);
	ssize_t		send(std::string msg);
	std::string	getIp(void);
	ssize_t		send_banner(std::string line);
	bool		isLogged(void);

	void		setCap(const std::string& cap);
	void		resetCap(const std::string& cap);
	bool		hasCap(const std::string& cap);
	void		setFlag(const std::string& cap);
	void		resetFlag(const std::string& cap);
	bool		hasFlag(const std::string& cap);
};

#endif //CLIENT_HPP
