/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:57:59 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/27 14:39:21 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
#include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return (1);
	}
	std::cout << "Starting IRC server..." << std::endl;

	Server	server;

	server.serverSetup();
	server.pollLoop();
	server.serverCleanup();

	/*server.initSocket();
	server.listenUp(0);
	server.pollRun();*/
	return 0;
}
