/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:38:24 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/07 12:25:12 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void)
{
	return ;
}

Server::~Server(void)
{
	return ;
}

std::string& Server::getName(void)
{
	return (this->_name);
}
in_port_t Server::getPort(void)
{
	return (ntohs(_IPv4_serv_sock_addr.sin_port));
}

std::string	Server::getIp(void)
{
	return (getLocalIPv4Address());
}

void	Server::server_init(int port, std::string passwd)
{
	this->_name = SERV_NAME;
	this->_passwd = passwd;
	memset(&this->_IPv4_serv_sock_addr, 0, sizeof(this->_IPv4_serv_sock_addr));
	this->_IPv4_serv_sock_addr.sin_family = AF_INET;
	this->_IPv4_serv_sock_addr.sin_port = htons(port);
	this->_IPv4_serv_sock_addr.sin_addr.s_addr = INADDR_ANY;
	this->_fd_server_socket = -1;
	memset(this->_buffer, 0, BUFFER_SIZE);
	this->_vector_clients.empty();
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
	this->_fd_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->_fd_server_socket == -1)
		{ throw (std::runtime_error("socket() error")); }
	if (fcntl(this->_fd_server_socket, F_SETFL, O_NONBLOCK) == -1)
		{ throw (std::runtime_error("fcntl() error")); }
	/*this->fd_server_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	if (this->fd_server_socket == -1)
		{ throw (std::runtime_error("socket() error")); }
	if (fcntl(this->fd_server_socket, F_SETFL, O_NONBLOCK) == -1)
		{ throw (std::runtime_error("fcntl() error")); }*/
	return ;
}

void				Server::setServerSockopt(void)
{
	int	opt_toggle = 1;
	
	if (setsockopt(this->_fd_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt_toggle, sizeof(opt_toggle)) == -1)
		{ throw (std::runtime_error("setsockopt() SO_REUSEADDR error")); }
	if (setsockopt(this->_fd_server_socket, SOL_SOCKET, SO_KEEPALIVE, &opt_toggle, sizeof(opt_toggle)) == -1)
		{ throw (std::runtime_error("setsockopt() SO_KEEPALIVE error")); }
	return ;
}

void				Server::bindServerSocket(void)
{
	if (bind(this->_fd_server_socket, (struct sockaddr *)&this->_IPv4_serv_sock_addr, sizeof(this->_IPv4_serv_sock_addr)) == -1)
		{ throw (std::runtime_error("bind() error")); }
	return ;
}

void				Server::listenServerSocket(void)
{
	if (listen(this->_fd_server_socket, QUEUE_SIZE) == -1)
		{ throw (std::runtime_error("listen() error")); }
	return ;
}

void				Server::serverCleanup(void)
{
	for (size_t i = 0; i < this->_vector_clients.size(); i++)
		{ this->_vector_clients[i].clientCleanup(); }
	this->_vector_clients.empty();
	if (this->_fd_server_socket != -1)
	{
		close(this->_fd_server_socket);
		this->_fd_server_socket = -1;
	}
	memset(&this->_IPv4_serv_sock_addr, 0, sizeof(this->_IPv4_serv_sock_addr));
	memset(this->_buffer, 0, BUFFER_SIZE);
	return ;
}

ssize_t	Server::sendHead(Client& cli, std::string nb) {
	return cli.send(":" + getName() + " " + nb + " " + cli.getNickname() + " ");
}

void	Server::handle(char *buffer, Client &client) {
    processCommand(*this, client, buffer);
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
	std::string			local_ipv4_addr;

	if (getifaddrs(&network_interfaces) == -1)
	{
		perror("getifaddrs");
		return ("NULL");
	}
	for (ptr_it_ifaddrs = network_interfaces; ptr_it_ifaddrs != NULL; ptr_it_ifaddrs = ptr_it_ifaddrs->ifa_next)
	{
		if (ptr_it_ifaddrs->ifa_addr == NULL)
			{ continue; }
		if (ptr_it_ifaddrs->ifa_addr->sa_family == AF_INET)
		{
			socket_address = (struct sockaddr_in *)ptr_it_ifaddrs->ifa_addr;
			local_ipv4_addr = inet_ntoa(socket_address->sin_addr);
			if (local_ipv4_addr != "127.0.0.1")
			{
				std::cout << "Detected local IPv4: " << local_ipv4_addr << std::endl;
				break;
			}
		}
	}
	freeifaddrs(network_interfaces);
	return (local_ipv4_addr);
}
