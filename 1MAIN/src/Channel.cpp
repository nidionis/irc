/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:20:21 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/09/16 14:15:27 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by nidionis on 04/07/25.
//

#include "Channel.hpp"

Channel::Channel(void)
{
	max_users = MAX_CLIENTS;
	return ;
}

Channel::~Channel(void)
{
	this->_clients.clear();
	this->_operators.clear();
	return ;
}

Channel::Channel(Client& client, std::string& name)
{
	this->_name = name;
	this->_clients.push_back(client);
	this->_operators.push_back(client);
}

bool	Channel::operator==(const Channel& other) const
{
	return (this->_name == other.getName());
}
Channel	&Channel::operator=(const Channel& other)
{
	if (this != &other)
	{
		this->_name = other.getName();
		this->_clients = other._clients;
		this->_operators = other._operators;
	}
	return (*this);
}

bool Channel::isOperator(Client& client)
{
	if (std::find(this->_operators.begin(), this->_operators.end(), client) != this->_operators.end())
		{ return (true); }
	else
		{ return (false); }
}

void Channel::setOperator(Client& client)
{
	if (this->isOperator(client))
		{ throw std::runtime_error("Client is already an operator\r\n"); }
	this->_operators.push_back(client);
	client.send("you operator the channel\r\n");
	return ;
}

void Channel::delOperator(Client& client)
{
	if (!this->isOperator(client))
		{ throw std::runtime_error("Client is not an operator"); }
	del(this->_operators, client);
	return ;
}


bool Channel::isClient(Client& client)
{
	if (std::find(this->_clients.begin(), this->_clients.end(), client) != this->_clients.end())
		{ return true; }
	else
		{ return false; }
}

void Channel::setClient(Client& client)
{
	if (this->isClient(client))
		{ throw std::runtime_error("Client is already in the channel\r\n"); }
	this->_clients.push_back(client);
	return ;
}

void Channel::delClient(Client& client)
{
	std::vector<Client>::iterator	it;

	if (!this->isClient(client))
		{ throw std::runtime_error("Client is not in the channel\r\n"); }
	it = std::find(this->_clients.begin(), this->_clients.end(), client);
	this->_clients.erase(it);
	return ;
}

bool	Channel::hasMode(char mode)
{
	return (is_in(this->_modes, mode));
}

void	Channel::setMode(char mode)
{
	print_vec(this->_modes);
	set(this->_modes, mode);
	print_vec(this->_modes);
	return ;
}

void	Channel::delMode(char mode)
{
	del(this->_modes, mode);
	return ;
}

void Channel::spawn(std::string msg)
{
	std::vector<Client>::iterator	it;
	std::vector<Client>::iterator	ite = this->_clients.end();

	for (it = this->_clients.begin(); it != ite; ++it) { (*it).send(msg); }
	return ;
}

bool	Channel::isEmpty()
{
	return _clients.empty();
}