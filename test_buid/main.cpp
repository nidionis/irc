/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:15:22 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/23 16:13:22 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 6667
#define BUFFER_SIZE 1024

int	main(void)
{
	struct sockaddr_in	address;
	char				buffer[BUFFER_SIZE] = {0};
	int					server_fd;
	int					new_socket;
	int					opt = 1;
	int					addrlen = sizeof(address);

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
	bind(server_fd, (struct sockaddr *)&address, sizeof(address));
	listen(server_fd, 3);
	printf("Server listening on port %d...\n", PORT);
	while (1)
	{
		new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
		printf("Client connected.\n");
		while (1)
		{
			memset(buffer, 0, BUFFER_SIZE);
			ssize_t bytes_read = recv(new_socket, buffer, BUFFER_SIZE, 0);
			if (bytes_read <= 0)
			{
				printf("Client disconnected.\n");
				break ;
			}
			printf("Received: %s", buffer);
		}
	}
	close(new_socket);
	close(server_fd);
	return (0);
}
