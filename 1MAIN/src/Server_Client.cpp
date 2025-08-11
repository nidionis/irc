/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_Client.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 14:18:11 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/10 15:07:51 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

ssize_t			Server::sendClient(Client& client, std::string msg)
{
	int	byte_sent;
	int	flags = MSG_DONTWAIT | MSG_NOSIGNAL;

	byte_sent = send(client.getfd_client_socket(), msg.c_str(), msg.size(), flags);
	if (byte_sent < 0)
		{ throw (std::runtime_error("sending client error")); }
	std::cout << client.getNickname() << msg << std::flush;
	return (byte_sent);
}

Client&	Server::getClient(int i)
{
	std::vector<Client>::iterator	it;
	std::vector<Client>::iterator	ite;

	ite = this->_vector_clients.end();
	for (it = this->_vector_clients.begin(); it != ite; ++it)
	{
		//i comes from p_data->i, which start at 1 (and i'ts dangerous)
		if (!--i) { return (*it); }
	}
	throw (std::runtime_error("client not found"));
}

Client&	Server::getClient(const std::string& nick)
{
	std::vector<Client>::iterator	it;
	std::vector<Client>::iterator	ite;

	ite = this->_vector_clients.end();
	for (it = this->_vector_clients.begin(); it != ite; ++it)
		{ if (it->getNickname() == nick) { return (*it); } }
	throw (std::runtime_error("client not found"));
}

void	Server::handleClient(char* buffer, Client& client)
{
	processCommand(*this, client, buffer);
	return ;
}

bool	Server::clientHasNick(std::string const& nick)
{
	std::string	name;

	for (unsigned int i = 0; i < this->_vector_clients.size(); ++i)
	{
		name = this->_vector_clients[i].getNickname();
		if (name == nick) {return (true); }
	}
	return (false);
}

bool	Server::clientHasUser(std::string const& username)
{
	std::string	name;

	for (unsigned int i = 0; i < this->_vector_clients.size(); ++i)
	{
		name = this->_vector_clients[i].getUsername();
		if (name == username) { return (true); }
	}
	return (false);
}
