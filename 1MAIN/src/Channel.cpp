/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:43:47 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/07 14:21:46 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by nidionis on 04/07/25.
//

#include "Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {
    this->clients.clear();
    this->operators.clear();
}

Channel::Channel(Client &client, std::string &name) {
    this->_name = name;
    this->clients.push_back(client);
    this->operators.push_back(client);
}

bool Channel::operator==(const Channel &other) const {
    return _name == other.getName();
}
Channel& Channel::operator=(const Channel& other) {
    if (this != &other) {
        _name = other.getName();
        clients = other.clients;
        operators = other.operators;
    }
    return *this;
}

bool Channel::isOperator(Client &client) {
    if (std::find(this->operators.begin(), this->operators.end(), client) != this->operators.end()) {
        return true;
    } else {
        return false;
    }
}

void Channel::setOperator(Client &client) {
    if (this->isOperator(client))
    {
        throw std::runtime_error("Client is already an operator");
    }
    this->operators.push_back(client);
    client.send("you operator the channel\r\n");
}

void Channel::delOperator(Client &client) {
    if (!this->isOperator(client)) {
        throw std::runtime_error("Client is not an operator");
    }
    del(this->operators, client);
}


bool Channel::isClient(Client &client) {
    if (std::find(this->clients.begin(), this->clients.end(), client) != this->clients.end()) {
        return true;
    } else {
        return false;
    }
}

void Channel::setClient(Client &client) {
    if (this->isClient(client))
    {
        throw std::runtime_error("Client is already in the channel");
    }
    this->clients.push_back(client);
}

void Channel::delClient(Client &client) {
    if (!this->isClient(client))
    {
        throw std::runtime_error("Client is not in the channel");
    }
    std::vector<Client>::iterator it = std::find(this->clients.begin(), this->clients.end(), client);
    this->clients.erase(it);
}

bool    Channel::hasMode(char mode) {
    return is_in(this->modes, mode);
}

void    Channel::setMode(char mode) {
    set(this->modes, mode);
}

void    Channel::delMode(char mode) {
    del(this->modes, mode);
}

void Channel::spawn(std::string msg)
{
    for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
    {
        (*it).send(msg);
    }
}

