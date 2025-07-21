//
// Created by nidionis on 04/07/25.
//

#include "../include/Channel.hpp"

#include <stdexcept>

Channel::Channel() {}

Channel::~Channel() {
    this->clients.clear();
    this->admins.clear();
}

Channel::Channel(Client &client, std::string &name) {
    this->name = name;
    this->clients.push_back(client);
    this->admins.push_back(client);
}

bool Channel::operator==(const Channel &other) const {
    return this->name == other.name;
}
Channel& Channel::operator=(const Channel& other) {
    if (this != &other) {
        name = other.name;
        clients = other.clients;
        admins = other.admins;
    }
    return *this;
}

bool Channel::isAdmin(Client &client) {
    if (std::find(this->admins.begin(), this->admins.end(), client) != this->admins.end()) {
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

void Channel::setAdmin(Client &client) {
    if (this->isAdmin(client))
    {
        throw std::runtime_error("Client is already an admin");
        return;
    }
    this->admins.push_back(client);
    client.send("you admin the channel\r\n");
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
