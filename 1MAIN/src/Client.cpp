/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:59:11 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/30 12:20:15 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
#include "Client.hpp"

/*Client::Client() : _nickname("default_name"), _username("default_username"),
_hostname("default_hostname"), _fd(UNDEFINED_FD) {
}*/

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

/*Client &Client::operator=(const Client &src) {
	if (this == &src)
		return *this;
	_nickname = src._nickname;
	_username = src._username;
	_hostname = src._hostname;
	_fd = src._fd;
	return *this;
}

void Client::setFd(int fd) {
	_fd = fd;
}

int Client::printing_loop() {
	unsigned char buffer[BUFF_SIZE];
	int bytes_read;
	while (true) {
		bytes_read = read(_fd, buffer, BUFF_SIZE - 1);
		if (bytes_read <= 0) {
			if (bytes_read < 0) {
				std::cerr << "Read failed: "
						  << strerror(errno)
						  << " (Error code: " << errno << ")"
						  << std::endl;
				return ERROR;
			} else {
				std::cerr << "Connection closed by peer" << std::endl;
				return QUIT;
			}
		}
		buffer[bytes_read] = '\0';
		std::cout << "Received: " << buffer;
	}
}*/
