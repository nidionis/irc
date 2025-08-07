/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:58:51 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:24 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include "capabilities.hpp"

# define MAX_CHANNELS 2
# define PASSWD_OK "passwd_ok"

# define GETTER_SETTER(type, name, f_name) \
type get##f_name() const { return name; } \
void set##f_name(type value) { name = value; }

class Channel;

class Client
{
private:
    Server              		*server;
    std::string         		_nickname; // NickName
    std::string         		_username; // Ident
    std::string         		_realname; // RealName
    std::vector<Channel>    	channels;
	std::vector<std::string>    capabilities;
    std::vector<std::string>    flags;

public:
	GETTER_SETTER(std::string, _nickname, Nickname)
	GETTER_SETTER(std::string, _username, Username)
	GETTER_SETTER(std::string, _realname, Realname)

	struct sockaddr_in	IPv4_client_sock_addr;
	int					fd_client_socket;
	socklen_t			client_addrlen;
    bool                must_kill;

	Client(void);
    Client(Server *server);
	~Client(void);
    bool operator==(const Client &other) const;
	void clientCleanup(void);

    Channel* newChannel(std::string& name);
    void delChannel(Channel& channel);
    ssize_t send(std::string msg);
    void setCap(const std::string& cap);
    void resetCap(const std::string& cap);
    bool hasCap(const std::string& cap);
    void setFlag(const std::string& cap);
    void resetFlag(const std::string& cap);
    bool hasFlag(const std::string& cap);
    std::string getIp();
    ssize_t send_banner(std::string line);
    bool isLogged();
};

//std::ostream& operator<<(std::ostream& os, const Client& client);

#endif //CLIENT_HPP
