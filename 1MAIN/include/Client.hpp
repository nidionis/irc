/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:58:51 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/30 11:56:00 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

//# define UNDEFINED_FD -1
//# define QUIT -2

class Client
{
private:

public:

	struct sockaddr_in	IPv4_client_sock_addr;
	int					fd_client_socket;
	socklen_t			client_addrlen;

	Client(void);
	~Client(void);

	void				clientCleanup(void);
};

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
	int printing_loop();
	
	struct sockaddr_in	IPv4_client_sock_addr;
	int					fd_client_socket;
	socklen_t			client_addrlen;*/

#endif //CLIENT_HPP
