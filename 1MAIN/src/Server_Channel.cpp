/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_Channel.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 14:19:00 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/31 13:55:43 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	Server::hasChannel(std::string const& nick)
{
	std::string	name;

	for (unsigned int i = 0; i < this->_vector_clients.size(); ++i)
	{
		name = this->_vector_clients[i].getUsername();
		if (name == nick) {return (true); }
	}
	return (false);
}

/*void	Server::pushChannel(Channel& channel)
{
	this->_channels.push_back(channel);
	return ;
}*/

void Server::pushChannel(Channel const &channel)
{
	this->_channels.push_back(channel); // copy by value
}

void	Server::delChannel(Channel& channel)
{
	std::vector<Channel>::iterator	it;

	it = std::find(this->_channels.begin(), this->_channels.end(), channel);
	if (it != this->_channels.end())
	{
		delete (&(*it));
		this->_channels.erase(it);
	}
	return ;
}

Channel	&Server::getChannel(std::string const& channel_str)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getName() == channel_str)
			{ return this->_channels[i]; }
	}
	throw (std::runtime_error("channel not found"));
}
