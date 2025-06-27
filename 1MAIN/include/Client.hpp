/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:58:51 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/27 14:50:43 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# define UNDEFINED_FD -1
# define QUIT -2

class Client
{
/*	std::string _nickname;
	std::string _username;
	std::string _hostname;
	int _fd;
public:
	Client();
	~Client();
	Client(Client &src);
	Client &operator=(const Client &src);

	void	setFd(int fd);
	int printing_loop();*/

private:
	/*struct sockaddr_in	IPV4_client_socket_address;
	int					fd_client_socket;
	socklen_t			client_addrlen;*/

public:
	Client();
	~Client();
};

#endif //CLIENT_HPP
