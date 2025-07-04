/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:59:11 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/30 13:39:46 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
#include "Client.hpp"

Client::Client(void) {
    throw (std::runtime_error("client must be set with a server"));
}

Client::Client(Server *server)
{
    this->server = server;
	memset(&this->IPv4_client_sock_addr, 0, sizeof(this->IPv4_client_sock_addr));
	this->client_addrlen = sizeof(this->IPv4_client_sock_addr);
	this->fd_client_socket = -1;
    this->_nickname = "";
    this->_username = "";
    this->_realname = "";
    this->_hostname = "";
	return ;
}

Client::~Client(void)
{
	return ;
}

bool Client::operator==(const Client &other) const {
    return this->fd_client_socket == other.fd_client_socket;
}

void				Client::clientCleanup(void)
{
	memset(&this->IPv4_client_sock_addr, 0, sizeof(this->IPv4_client_sock_addr));
	this->client_addrlen = sizeof(this->IPv4_client_sock_addr);
	if (this->fd_client_socket != -1)
	{
		close(this->fd_client_socket);
		this->fd_client_socket = -1;
	}
	return ;
}

std::string	&Client::getNickname(void) { return this->_nickname; }
void    	Client::setNickname(std::string buffer) { this->_nickname = buffer; }
std::string &Client::getUsername(void) { return this->_username; }
void    	Client::setUsername(std::string buffer) { this->_username = buffer; }
std::string &Client::getRealname(void) { return this->_realname; }
void    	Client::setRealname(std::string buffer) { this->_realname = buffer; }
std::string &Client::getHostname(void) { return this->_hostname; }
void    	Client::setHostname(std::string buffer) { this->_hostname = buffer; }

Channel*	Client::newChannel(std::string &name) {
    Channel *channel = new Channel(*this, name);
    this->server->pushChannel(*channel);
    this->channels.push_back(*channel);
    return channel;
}

ssize_t Client::send(std::string msg) {
    return (this->server->sendClient(*this, msg));
}
