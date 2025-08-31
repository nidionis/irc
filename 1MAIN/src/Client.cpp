/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:20:28 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/31 14:28:03 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void)
{
	throw (std::runtime_error("client must be set with a server"));
}

Client::Client(Server* server)
{
	this->_server = server;
	memset(&this->_IPv4_client_sock_addr, 0, sizeof(this->_IPv4_client_sock_addr));
	this->_client_addrlen = sizeof(this->_IPv4_client_sock_addr);
	this->_fd_client_socket = -1;
	this->_nickname = "";
	this->_username = "";
	this->_realname = "";
	this->_must_kill = false;
	if (this->_server->checkPasswd(""))
		{ this->setFlag(PASSWD_OK); }
	return;
}

Client::~Client(void)
{
	return;
}

bool	Client::operator==(const Client& other) const
{
	return (this->_fd_client_socket == other._fd_client_socket);
}

struct sockaddr_in&	Client::special_getIPv4_client_sock_addr(void)
{
	return (this->_IPv4_client_sock_addr);
}

socklen_t&			Client::special_get_client_addrlen(void)
{
	return (this->_client_addrlen);
}

void	Client::clientCleanup(void)
{
	memset(&this->_IPv4_client_sock_addr, 0, sizeof(this->_IPv4_client_sock_addr));
	this->_client_addrlen = sizeof(this->_IPv4_client_sock_addr);
	if (this->_fd_client_socket != -1)
	{
		close(this->_fd_client_socket);
		this->_fd_client_socket = -1;
	}
	return ;
}

/*void	Client::newChannel(std::string& name)
{
	Channel	*channel;
	
	if (this->_channels.size() >= MAX_CHANNELS)
		{ throw (std::runtime_error("too many channels")); }
	channel = new Channel(*this, name);
	this->_server->pushChannel(*channel);
	this->_channels.push_back(*channel);
	this->send("JOIN : Succefully created\r\n");
	delete (channel);
	return ;
}*/

void Client::newChannel(std::string& name)
{
	if (this->_channels.size() >= MAX_CHANNELS)
		throw std::runtime_error("too many channels");
	
	Channel channel(*this, name);

	this->_server->pushChannel(channel);
	this->_channels.push_back(channel);
	this->send("JOIN : Successfully created\r\n");
	return ;
}

/*void	Client::delChannel(Channel& channel)
{
	std::vector<Channel>::iterator	it;
	
	it = std::find(this->_channels.begin(), this->_channels.end(), channel);
	this->_channels.erase(it);
	this->_server->delChannel(channel);
}*/

ssize_t	Client::send(std::string msg)
{
	return (this->_server->sendClient(*this, msg));
}

ssize_t	Client::send_banner(std::string line)
{
	static size_t	i = 0;
	ssize_t			byte_sent = 0;

	byte_sent += this->send(this->_server->getName() + " " + to_string(i) + " " + line);
	i++;
	return (byte_sent);
}

void	Client::setCap(const std::string& cap)
{
	if (isCap(cap))
	{
		if (!is_in(this->_capabilities, cap))
			{ this->_capabilities.push_back(cap); }
	}
}

void	Client::resetCap(const std::string& cap)
{
	std::vector<std::string>::iterator	it;

	if (isCap(cap))
	{
		if (is_in(this->_capabilities, cap))
		{
			it = std::find(this->_capabilities.begin(), this->_capabilities.end(), cap);
			this->_capabilities.erase(it);
			this->send("CAP REQ : " + cap + " reset\r\n");
		}
	}
}

bool	Client::hasCap(const std::string& cap)
{
	return (is_in(this->_capabilities, cap));
}

void	Client::setFlag(const std::string& flag)
{
	if (!is_in(this->_flags, flag))
		{ this->_flags.push_back(flag); }
}

void	Client::resetFlag(const std::string& flag)
{
	std::vector<std::string>::iterator	it;

	if (is_in(this->_flags, flag))
	{
		it = std::find(this->_flags.begin(), this->_flags.end(), flag);
		this->_flags.erase(it);
	}
}

bool	Client::hasFlag(const std::string& flag)
{
	return is_in(this->_flags, flag);
}

std::string Client::getIp(void)
{
	return (inet_ntoa(this->_IPv4_client_sock_addr.sin_addr));
}

bool	Client::isLogged(void)
{
	std::cout << "getUsername() : [" + getUsername() << "]" << std::endl
			  << "getNickname() : [" + getNickname() << "]" << std::endl
			  << "hasFlag(PASSWD_OK) : [" << hasFlag(PASSWD_OK) << "]" << std::endl
			  << "hasFlag(\"CAP_END\") : [" << hasFlag("CAP_END") << "]" << std::endl;
	return (getUsername() != "" && getNickname() != "" && hasFlag(PASSWD_OK) && hasFlag("CAP_END"));
}

Channel &Client::getChannel(void) {
    return _channels.front();
}
