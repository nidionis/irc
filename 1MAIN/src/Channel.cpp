//
// Created by nidionis on 04/07/25.
//

#include "../include/Channel.hpp"

#include <stdexcept>
#include <vector>

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

bool Channel::isClient(Client &client) {
    if (std::find(this->clients.begin(), this->clients.end(), client) != this->clients.end()) {
        return true;
    } else {
        return false;
    }
}

void Channel::setOperator(Client &client) {
    if (this->isOperator(client))
    {
        throw std::runtime_error("Client is already an operator");
        return;
    }
    this->operators.push_back(client);
    client.send("you operator the channel\r\n");
}

void Channel::setClient(Client &client) {
    if (this->isClient(client))
    {
        throw std::runtime_error("Client is already in the channel");
        return;
    }
    this->clients.push_back(client);
    client.send("you joined the channel\r\n");
}

void Channel::delClient(Client &client) {
    if (!this->isClient(client))
    {
        throw std::runtime_error("Client is not in the channel");
        return;
    }
    std::vector<Client>::iterator it = std::find(this->clients.begin(), this->clients.end(), client);
    this->clients.erase(it);
}

bool    Channel::hasFlag(std::string flag) {
    return is_in(this->flags, flag);
}

void    Channel::setFlag(std::string flag) {
    set(this->flags, flag);
}

void    Channel::delFlag(std::string flag) {
    del(this->flags, flag);
}

void Channel::spawn(std::string msg)
{
    for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
    {
        (*it).send(msg);
    }
}
