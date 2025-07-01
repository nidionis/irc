/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:57:59 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/30 14:14:20 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
#include "Server.hpp"

int main(int argc, char **argv)
{
	Server	server;
	
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return (1);
	}
	std::cout << "Starting IRC server..." << std::endl;
	try { server.serverSetup(); }
	catch (const std::exception& err) { std::cout << err.what() << std::endl; }
	try { server.pollLoop(); }
	catch (const std::exception& err) { std::cout << err.what() << std::endl; }
	server.serverCleanup();
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

bool cmpHead(const std::string str1, const std::string str2) {
    std::string wd1 = str1;
    std::string wd2 = str2;

    std::transform(wd1.begin(), wd1.end(), wd1.begin(), ::tolower);
    std::transform(wd2.begin(), wd2.end(), wd2.begin(), ::tolower);
    std::stringstream ss1(wd1);
    std::stringstream ss2(wd2);
    ss1 >> wd1;
    ss2 >> wd2;
    return wd1 == wd2;
}
