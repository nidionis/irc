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

Client::Client(void)
{
	memset(&this->IPv4_client_sock_addr, 0, sizeof(this->IPv4_client_sock_addr));
	this->client_addrlen = sizeof(this->IPv4_client_sock_addr);
	this->fd_client_socket = -1;
	return ;
}

Client::~Client(void)
{
	return ;
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
void    	Client::setNickname(char *buffer) { this->_nickname = buffer; }
std::string &Client::getUsername(void) { return this->_username; }
void    	Client::setUsername(char *buffer) { this->_username = buffer; }
std::string &Client::getRealname(void) { return this->_realname; }
void    	Client::setRealname(char *buffer) { this->_realname = buffer; }
std::string &Client::getHostname(void) { return this->_hostname; }
void    	Client::setHostname(char *buffer) { this->_hostname = buffer; }
