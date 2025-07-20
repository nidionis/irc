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
# define USER_PREFIXES "+%@!."
# define VOICED     0 // This prefix indicates a voiced user, who can speak while the channel is being moderated.
# define HALF       1 // This prefix denotes a half-operator, who has limited privileges compared to a full operator.
# define CHANNEL    2 // This prefix is used for channel operators, who have the ability to kick users, change modes, and manage the channel.
# define ADMIN      3 // This prefix is given by servers or services and denotes that the user is a service, typically the highest ranking membership type in the channel.
# define FOUNDER    4 // This prefix is used for founders, who have a rank higher than a normal operator
//exemple of use: USER_PREFIXES[ADMIN]

// # define CHANNEL_PREFIXES "#&!+.~"
// # define WIDE_CHAN      0 //: This is the standard prefix for network-wide channels, which are known to all servers connected to the network.
// # define LOCAL_CHAN     1 //: This prefix is used for server-specific or local channels, which are only visible to clients on the same server.
// # define TIMESTAMP_CHAN 2 //: This prefix denotes a timestamped channel on normally non-timestamped networks.
// # define ARNAR_CHAN     3 //: This prefix is used for unmoderated channels, where no one can obtain channel operator status.
// # define BOT_CHAN       3 //: This is a programmable channel type, typically controlled by software, and not creatable by normal users.

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
    void                delChannel(Channel &channel);
    ssize_t             send(std::string msg);
};

#endif //CLIENT_HPP
