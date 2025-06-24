/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:15:22 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/24 12:52:02 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*  printf() : <stdio.h>
    memset() : <string.h>
     close() : <unistd.h>
     htons() : <arpa/inet.h>
    socket() : <sys/socket.h>
setsockopt() : <sys/socket.h>
      bind() : <sys/socket.h>
    listen() : <sys/socket.h>
    accept() : <sys/socket.h>
      recv() : <sys/socket.h>
	  poll() : <poll.h>*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>

#define PORT 6667
#define BUFFER_SIZE 1024

int	main(void)
{
	struct sockaddr_in	address;
	char				buffer[BUFFER_SIZE] = {0};
	int					fd_server_socket;
	int					fd_client_socket;
	int					opt = 1;
	int					addrlen = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	fd_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(fd_server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	bind(fd_server_socket, (struct sockaddr *)&address, sizeof(address));
	listen(fd_server_socket, 3);
	printf("Server listening on port %d...\n", PORT);
	while (1)
	{
		fd_client_socket = accept(fd_server_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);
		printf("Client connected.\n");
		while (1)
		{
			memset(buffer, 0, BUFFER_SIZE);
			ssize_t bytes_read = recv(fd_client_socket, buffer, BUFFER_SIZE, 0);
			if (bytes_read <= 0)
			{
				printf("Client disconnected.\n");
				break ;
			}
			printf("Received: %s", buffer);
		}
	}
	close(fd_client_socket);
	close(fd_server_socket);
	return (0);
}
