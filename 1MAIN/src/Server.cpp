/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:38:24 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/03 15:29:56 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
#include "Server.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <bits/ostream.tcc>

#include "Client.hpp"
#include "Handle.hpp"
#include "../include/Channel.hpp"
#include "../include/Client.hpp"

void processCommand(Server &server, Client &client, std::string input);
std::string	getLocalIPv4Address(void);

Server::Server()
{
	return ;
}

Server::~Server(void)
{
	return ;
}

std::string &Server::getName(void) {
    return this->_name;
}

void Server::server_init(int port, std::string passwd)
{
	this->_name = SERV_NAME;
	this->_passwd = passwd;
	memset(&this->IPv4_serv_sock_addr, 0, sizeof(this->IPv4_serv_sock_addr));
	this->IPv4_serv_sock_addr.sin_family = AF_INET;
	this->IPv4_serv_sock_addr.sin_port = htons(port);
	this->IPv4_serv_sock_addr.sin_addr.s_addr = INADDR_ANY;
	this->fd_server_socket = -1;
	memset(this->buffer, 0, BUFFER_SIZE);
	this->vector_clients.empty();
}

void	Server::serverSetup(void)
{
	initServerSocket();
	setServerSockopt();
	bindServerSocket();
	listenServerSocket();
	return ;
}

void				Server::initServerSocket(void)
{
	// SOCK_STREAM | SOCK_NONBLOCK
	this->fd_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->fd_server_socket == -1) {
        throw (std::runtime_error("socket() error"));
    } if (fcntl(this->fd_server_socket, F_SETFL, O_NONBLOCK) == -1) {
        throw (std::runtime_error("fcntl() error"));
    }
    /*this->fd_server_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	if (this->fd_server_socket == -1) {
        throw (std::runtime_error("socket() error"));
    } if (fcntl(this->fd_server_socket, F_SETFL, O_NONBLOCK) == -1) {
        throw (std::runtime_error("fcntl() error"));
    }*/
	return ;
}

void				Server::setServerSockopt(void)
{
	int	opt_toggle = 1;
	
	if (setsockopt(this->fd_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt_toggle, sizeof(opt_toggle)) == -1) {
        throw (std::runtime_error("setsockopt() SO_REUSEADDR error"));
    } if (setsockopt(this->fd_server_socket, SOL_SOCKET, SO_KEEPALIVE, &opt_toggle, sizeof(opt_toggle)) == -1) {
        throw (std::runtime_error("setsockopt() SO_KEEPALIVE error"));
    }
	return ;
}

void				Server::bindServerSocket(void)
{
	if (bind(this->fd_server_socket, (struct sockaddr *)&this->IPv4_serv_sock_addr, sizeof(this->IPv4_serv_sock_addr)) == -1) {
        throw (std::runtime_error("bind() error"));
    }
	return ;
}

void				Server::listenServerSocket(void)
{
	if (listen(this->fd_server_socket, QUEUE_SIZE) == -1)
		{ throw (std::runtime_error("listen() error")); }
	return ;
}

void				Server::serverCleanup(void)
{
	for (size_t i = 0; i < this->vector_clients.size(); i++)
		{ this->vector_clients[i].clientCleanup(); }
	this->vector_clients.empty();
	if (this->fd_server_socket != -1)
	{
		close(this->fd_server_socket);
		this->fd_server_socket = -1;
	}
	memset(&this->IPv4_serv_sock_addr, 0, sizeof(this->IPv4_serv_sock_addr));
	memset(this->buffer, 0, BUFFER_SIZE);
	return ;
}

ssize_t 		Server::sendClient(Client &cli, std::string msg) {
    int byte_sent;
    int flags = MSG_DONTWAIT | MSG_NOSIGNAL;
    byte_sent = send(cli.fd_client_socket, msg.c_str(), msg.size(), flags);
    if (byte_sent < 0) {
        throw (std::runtime_error("sending client error"));
    }
	std::cout << ">> to " << cli.getNickname() << " >> : " << msg << std::endl;
    return byte_sent;
}

Client&	Server::getClient(int i) {
    for (std::vector<Client>::iterator it = vector_clients.begin(); it != vector_clients.end(); ++it) {
        if (!--i) { //i comes from p_data->i, which start at 1 (and i'ts dangerous)
            return *it;
        }
    }
    throw (std::runtime_error("client not found"));
}

Client &Server::getClient(const std::string &nick)
{
	for (std::vector<Client>::iterator it = vector_clients.begin(); it != vector_clients.end(); ++it) {
		if (it->getNickname() == nick) {
			return *it;
		}
	}
	throw (std::runtime_error("client not found"));
}

std::string	Server::getIp(void)
{
	return (getLocalIPv4Address());
}

void	Server::handle(char *buffer, Client &client) {
    processCommand(*this, client, buffer);
}

bool	Server::hasNick(std::string const &nick)
{
	std::string	name;
	for (unsigned int i = 0; i < this->vector_clients.size(); ++i)
	{
		name = this->vector_clients[i].getNickname();
		if (name == nick)
			return (true);
	}
	return (false);
}

bool	Server::hasUser(std::string const &username)
{
	std::string	name;
	for (unsigned int i = 0; i < this->vector_clients.size(); ++i)
	{
		name = this->vector_clients[i].getUsername();
		if (name == username)
			return (true);
	}
	return (false);
}

bool	Server::hasChannel(std::string const &nick)
{
	std::string	name;
	for (unsigned int i = 0; i < this->vector_clients.size(); ++i)
	{
		name = this->vector_clients[i].getUsername();
		if (name == nick)
			return (true);
	}
	return (false);
}

void Server::pushChannel(Channel &channel) {
    this->channels.push_back(channel);
}

void Server::delChannel(Channel &channel) {
    std::vector<Channel>::iterator it = std::find(channels.begin(), channels.end(), channel);
    if (it != channels.end()) {
        delete &(*it);
        channels.erase(it);
    }
}

Channel	&Server::getChannel(std::string const &channel_str)
{
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		if (this->channels[i].getName() == channel_str)
		{
			return this->channels[i];
		}
	}
	throw (std::runtime_error("channel not found"));
}

bool	Server::checkPasswd(std::string passwd)
{
	std::cout << "passwd: " << this->_passwd << std::endl;
	if (passwd == this->_passwd)
		return (true);
	return (false);
}

std::string	getLocalIPv4Address(void)
{
    struct ifaddrs		*network_interfaces;
    struct ifaddrs		*ptr_it_ifaddrs;
    struct sockaddr_in	*socket_address;
    std::string 		local_ipv4_addr;

    if (getifaddrs(&network_interfaces) == -1)
	{
        perror("getifaddrs");
        return ("NULL");
    }
    for (ptr_it_ifaddrs = network_interfaces; ptr_it_ifaddrs != NULL; ptr_it_ifaddrs = ptr_it_ifaddrs->ifa_next)
	{
        if (ptr_it_ifaddrs->ifa_addr == NULL)
            continue;
        if (ptr_it_ifaddrs->ifa_addr->sa_family == AF_INET)
		{
            socket_address = (struct sockaddr_in *)ptr_it_ifaddrs->ifa_addr;
            local_ipv4_addr = inet_ntoa(socket_address->sin_addr);
            if (local_ipv4_addr != "127.0.0.1") { std::cout << "Detected local IPv4: " << local_ipv4_addr << std::endl; break; }
        }
    }
    freeifaddrs(network_interfaces);
	return (local_ipv4_addr);
}
