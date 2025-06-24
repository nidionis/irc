/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:34:07 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/24 16:36:10 by lahlsweh         ###   ########.fr       */
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
	  recv() : <sys/socket.h>*/

/*typedef unsigned long socklen_t

typedef struct in_addr { unsigned long	s_addr; };

typedef struct	sockaddr
{
	unsigned short	sa_family;		// Common data: address family and length.
	char			sa_data[14];	// Address data.
};

//Structure describing an Internet socket address.
typedef struct sockaddr_in
{
	unsigned short	sin_family;
	unsigned short	sin_port;	// Port number.
	struct in_addr	sin_addr;	// Internet address.
	// Pad to size of `struct sockaddr'.
	unsigned char	sin_zero[sizeof(sockaddr) - sizeof(unsigned short) - sizeof(unsigned short) - sizeof(in_addr)];
};*/

/*
AF_INET -> stands for Address Family: Internet (IPv4)

htons(PORT) used because:
Host byte order: The byte order your machine uses internally.
	On many common machines like x86 (Intel/AMD), this is little-endian (Least Significant Byte first).
Network byte order: The standardized byte order used in network protocols,
	which is big-endian (Most Significant Byte first).

INADDR_ANY -> "Address to accept any incoming messages"

SOCK_STREAM -> "Sequenced, reliable, connection-based byte streams."
	typically used in TCP connections, like our IRC server

IPPROTO_TCP -> "Transmission Control Protocol." (for TCP/IPv4)

NOTE : setsockopt() can only set one option at a time !
the __level parameter tells the system which protocol layer the option belongs to
the __optname parameter indicates the options applied to said __level
level  : SOL_SOCKET -> stands for "socket-level" option level
option : SO_REUSEADDR -> Allows immediate reuse of a port after program exit
option : SO_KEEPALIVE -> Sends keepalive packets on TCP connections to detect dead clients

bind() associates a socket with a specific IP address and port number

memset() is used to properly empty and cleanup buffer before subsequent recv() calls. fills buffer data with '\0'

recv() is used to receive data from client and assign it to buffer
MSG_DONTWAIT -> Make the call non-blocking. useful for multi-client handling*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#define PORT 6667
#define BUFFER_SIZE 1024
#define QUEUE_SIZE 16 // Must be <= SOMAXCONN

int	main(void)
{
	struct sockaddr_in	IPV4_server_socket_address;
	struct sockaddr_in	IPV4_client_socket_address;
	int					fd_server_socket;
	int					fd_client_socket;
	char				client_ip4[INET_ADDRSTRLEN];
	char				buffer[BUFFER_SIZE] = {0};
	int					opt_toggle = 1;
	socklen_t			client_addrlen = sizeof(IPV4_client_socket_address);
	ssize_t				bytes_read;

	IPV4_server_socket_address.sin_family = AF_INET;
	IPV4_server_socket_address.sin_port = htons(PORT);
	IPV4_server_socket_address.sin_addr.s_addr = INADDR_ANY;
	fd_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	setsockopt(fd_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt_toggle, sizeof(opt_toggle));
	setsockopt(fd_server_socket, SOL_SOCKET, SO_KEEPALIVE, &opt_toggle, sizeof(opt_toggle));
	bind(fd_server_socket, (struct sockaddr *)&IPV4_server_socket_address, sizeof(IPV4_server_socket_address));
	listen(fd_server_socket, QUEUE_SIZE);
	printf("Server listening on port %d...\n", PORT);

	while (1)
	{
		bytes_read = recv(fd_client_socket, buffer, (BUFFER_SIZE - 1), MSG_DONTWAIT);
		if (bytes_read == 0) { printf("Client disconnected.\n"); break; }
		else if (bytes_read < 0)
		{
			// No data available now, just continue (or maybe sleep a bit)
			if (errno == EAGAIN || errno == EWOULDBLOCK) { continue ; }
			else { perror("recv"); break ; }
		}
		else
		{
			buffer[bytes_read] = '\0';
			printf("Received: %s", buffer);
		}
	}
	close(fd_client_socket);
	close(fd_server_socket);
	return (0);
}
