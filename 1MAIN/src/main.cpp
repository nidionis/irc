/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:57:59 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/31 13:27:58 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int main(int argc, char** argv)
{
	Server	server;

	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return (1);
	}
	std::cout << "Starting IRC server..." << std::endl;
	std::string	passwd(argv[2]);
	server.server_init(atoi(argv[1]), passwd);
	try { server.serverSetup(); }
	catch (const std::exception& err) { std::cerr << err.what() << std::endl; }
	try { server.pollLoop(); }
	catch (const std::exception& err) { std::cerr << err.what() << std::endl; }
	//server.serverCleanup();
	return (0);
}

void	pollDataCleanup(poll_data* poll_data)
{
	for (int i = 0; i < poll_data->fd_nb; ++i)
	{
		if (poll_data->fds[i].fd >= 0)
		{
			close(poll_data->fds[i].fd);
			poll_data->fds[i].fd = -1;
		}
	}
	poll_data->fd_nb = 0;
	poll_data->i = 0;
	poll_data->err_check = 0;
	return ;
}
