/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:59:11 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/30 13:39:46 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
#include "Client.hpp"

Client::Client(void)
{
	memset(&this->IPv4_client_sock_addr, 0, sizeof(this->IPv4_client_sock_addr));
	this->client_addrlen = sizeof(this->IPv4_client_sock_addr);
	this->fd_client_socket = -1;
	return ;
}

Client::~Client(void)
{
	return ;
}

void				Client::clientCleanup(void)
{
	memset(&this->IPv4_client_sock_addr, 0, sizeof(this->IPv4_client_sock_addr));
	this->client_addrlen = sizeof(this->IPv4_client_sock_addr);
	if (this->fd_client_socket != -1)
	{
		close(this->fd_client_socket);
		this->fd_client_socket = -1;
	}
	return ;
}
