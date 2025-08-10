/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:20:21 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/10 12:03:06 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by nidionis on 04/07/25.
//

#include "Channel.hpp"

Channel::Channel(void)
{
	return ;
}

Channel::~Channel(void)
{
	this->clients.clear();
	this->operators.clear();
}

Channel::Channel(Client &client, std::string &name)
{
	this->_name = name;
	this->clients.push_back(client);
	this->operators.push_back(client);
}

bool	Channel::operator==(const Channel &other) const
{
	return (_name == other.getName());
}
Channel	&Channel::operator=(const Channel& other)
{
	if (this != &other)
	{
		_name = other.getName();
		clients = other.clients;
		operators = other.operators;
	}
	return (*this);
}

bool Channel::isOperator(Client &client)
{
	if (std::find(this->operators.begin(), this->operators.end(), client) != this->operators.end())
		{ return (true); }
	else
		{ return (false); }
}

void Channel::setOperator(Client &client)
{
	if (this->isOperator(client))
		{ throw std::runtime_error("Client is already an operator"); }
	this->operators.push_back(client);
	client.send("you operator the channel\r\n");
	return ;
}

void Channel::delOperator(Client &client)
{
	if (!this->isOperator(client))
		{ throw std::runtime_error("Client is not an operator"); }
	del(this->operators, client);
	return ;
}


bool Channel::isClient(Client &client)
{
	if (std::find(this->clients.begin(), this->clients.end(), client) != this->clients.end())
		{ return true; }
	else
		{ return false; }
}

void Channel::setClient(Client &client)
{
	if (this->isClient(client))
		{ throw std::runtime_error("Client is already in the channel"); }
	this->clients.push_back(client);
	return ;
}

void Channel::delClient(Client &client)
{
	if (!this->isClient(client))
		{ throw std::runtime_error("Client is not in the channel"); }
	std::vector<Client>::iterator it = std::find(this->clients.begin(), this->clients.end(), client);
	this->clients.erase(it);
	return ;
}

bool	Channel::hasMode(char mode)
{
	return (is_in(this->modes, mode));
}

void	Channel::setMode(char mode)
{
	set(this->modes, mode);
	return ;
}

void	Channel::delMode(char mode)
{
	del(this->modes, mode);
	return ;
}

void Channel::spawn(std::string msg)
{
	std::vector<Client>::iterator it;
	std::vector<Client>::iterator ite = this->clients.end();

	for (it = this->clients.begin(); it != ite; ++it) { (*it).send(msg); }
	return ;
}

