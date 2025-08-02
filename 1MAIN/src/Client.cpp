/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:59:11 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/02 15:50:34 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include <stdexcept>

Client::Client(void)
{
    throw (std::runtime_error("client must be set with a server"));
}

Client::Client(Server* server)
{
    this->server = server;
    memset(&this->IPv4_client_sock_addr, 0, sizeof(this->IPv4_client_sock_addr));
    this->client_addrlen = sizeof(this->IPv4_client_sock_addr);
    this->fd_client_socket = -1;
    this->_nickname = "";
    this->_username = "";
    this->_realname = "";
    this->must_kill = false;
    if (this->server->checkPasswd("")) {
        this->setFlag(PASSWD_OK);
    }
    return;
}

Client::~Client(void)
{
    return;
}

bool Client::operator==(const Client& other) const
{
    return this->fd_client_socket == other.fd_client_socket;
}

void Client::clientCleanup(void)
{
    memset(&this->IPv4_client_sock_addr, 0, sizeof(this->IPv4_client_sock_addr));
    this->client_addrlen = sizeof(this->IPv4_client_sock_addr);
    if (this->fd_client_socket != -1)
    {
        close(this->fd_client_socket);
        this->fd_client_socket = -1;
    }
    return;
}

Channel* Client::newChannel(std::string& name)
{
    if (this->channels.size() >= MAX_CHANNELS)
    {
        throw (std::runtime_error("too many channels"));
    }
    Channel* channel = new Channel(*this, name);
    this->server->pushChannel(*channel);
    this->channels.push_back(*channel);
    this->send("JOIN : Succefully created\r\n");
    return channel;
}

void Client::delChannel(Channel& channel) {
    std::vector<Channel>::iterator it = std::find(channels.begin(), channels.end(), channel);
    channels.erase(it);
    this->server->delChannel(channel);
}

ssize_t Client::send(std::string msg)
{
    return (this->server->sendClient(*this, msg));
}

ssize_t Client::send_banner(std::string line)
{
    static size_t i = 0;
    ssize_t byte_sent = 0;
    byte_sent += this->send(this->server->getName());
    byte_sent += this->send(" ");
    byte_sent += this->send(to_string(i));
    byte_sent += this->send(" ");
    byte_sent += this->send(line);
    i++;
    return (byte_sent);
}

void	Client::setCap(const std::string &cap)
{
    if (isCap(cap))
    {
        if (!is_in(this->capabilities, cap))
        {
            this->capabilities.push_back(cap);
            this->send(":");
            this->send(this->server->getName());
            this->send(" CAP ");
            this->send(this->getNickname());
            this->send(" ACK :");
            this->send(cap + "\r\n");
            }
    }
}

void Client::resetCap(const std::string &cap)
{
    if (isCap(cap))
    {
        if (is_in(this->capabilities, cap))
        {
            std::vector<std::string>::iterator it = std::find(this->capabilities.begin(), this->capabilities.end(), cap);
            this->capabilities.erase(it);
            this->send("CAP REQ : " + cap + " reset\r\n");
        }
    }
}

bool    Client::hasCap(const std::string &cap)
{
    return is_in(this->capabilities, cap);
}

void	Client::setFlag(const std::string &flag)
{
    if (is_in(this->flags, flag)) {
        //this->send("[debug] flag " + flag + " was already set\r\n");
    } else {
        this->flags.push_back(flag);
        //this->send("[debug] flag " + flag + " set\r\n");
    }
}

void Client::resetFlag(const std::string &flag)
{
    if (is_in(this->flags, flag)) {
        std::vector<std::string>::iterator it = std::find(this->flags.begin(), this->flags.end(), flag);
        this->flags.erase(it);
        //this->send("[debug] flag " + flag + " reset\r\n");
    } else {
        //this->send("[debug] flag " + flag + " was not set\r\n");
    }
}

bool    Client::hasFlag(const std::string &flag)
{
    return is_in(this->flags, flag);
}

std::string Client::getIp() {
    return (inet_ntoa(this->IPv4_client_sock_addr.sin_addr));
}
