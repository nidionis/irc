/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_main.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 13:52:54 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/27 17:43:06 by lahlsweh         ###   ########.fr       */
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
	  poll() : <poll.h>
	 fcntl() : <unistd.h>, <fcntl.h>*/

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

/*AF_INET -> stands for Address Family: Internet (IPv4)

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
MSG_DONTWAIT -> Make the call non-blocking. useful for multi-client handling

fcntl() is used to ensure the fd_sockets are non-blocking

poll() is used to handle several connections simultaneously*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <poll.h>
#include <errno.h>
#include <fcntl.h>

#define PORT 6667
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 3
#define MAX_CONNECTIONS (MAX_CLIENTS + 1)
#define QUEUE_SIZE 16

int	main(void)
{
	struct sockaddr_in	IPV4_server_socket_address;
	struct sockaddr_in	IPV4_client_socket_address;
	int					fd_server_socket;
	int					fd_client_socket;
	char				buffer[BUFFER_SIZE];
	int					opt_toggle = 1;
	socklen_t			client_addrlen;
	ssize_t				recv_read;
	struct pollfd		poll_fds[MAX_CONNECTIONS];
	int					poll_fd_nb = 1;
	int					poll_err_check;
	
	memset(&IPV4_server_socket_address, 0, sizeof(IPV4_server_socket_address));
	memset(&IPV4_client_socket_address, 0, sizeof(IPV4_client_socket_address));
	IPV4_server_socket_address.sin_family = AF_INET;
	IPV4_server_socket_address.sin_port = htons(PORT);
	IPV4_server_socket_address.sin_addr.s_addr = INADDR_ANY;
	fd_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	fcntl(fd_server_socket, F_SETFL, O_NONBLOCK);
	setsockopt(fd_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt_toggle, sizeof(opt_toggle));
	setsockopt(fd_server_socket, SOL_SOCKET, SO_KEEPALIVE, &opt_toggle, sizeof(opt_toggle));
	bind(fd_server_socket, (struct sockaddr *)&IPV4_server_socket_address, sizeof(IPV4_server_socket_address));
	listen(fd_server_socket, QUEUE_SIZE);
	printf("Server listening on port %d...\n", PORT);
	poll_fds[0].fd = fd_server_socket;
	poll_fds[0].events = POLLIN;
	while (1)
	{
		poll_err_check = poll(poll_fds, poll_fd_nb, -1);
		if (poll_err_check == -1 && errno == EINTR) { continue ; }
		for (int i = 0; i < poll_fd_nb; i++)
		{
			if (poll_fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
			{
				printf("Error/hangup on fd %d: Closing.\n", poll_fds[i].fd);
				close(poll_fds[i].fd);
				poll_fds[i] = poll_fds[poll_fd_nb - 1];
				poll_fd_nb--;
				i--;
				continue ;
			}
			else if (poll_fds[i].revents & POLLIN)
			{
				if (poll_fds[i].fd == fd_server_socket)
				{
					client_addrlen = sizeof(IPV4_client_socket_address);
					fd_client_socket = accept(fd_server_socket, (struct sockaddr *)&IPV4_client_socket_address, &client_addrlen);
					fcntl(fd_client_socket, F_SETFL, O_NONBLOCK);
					if (poll_fd_nb < MAX_CONNECTIONS)
					{
						poll_fds[poll_fd_nb].fd = fd_client_socket;
						poll_fds[poll_fd_nb].events = POLLIN;
						poll_fd_nb++;
						printf("Client connected: %s:%d\n", inet_ntoa(IPV4_client_socket_address.sin_addr),
							ntohs(IPV4_client_socket_address.sin_port));
					}
					else { printf("Error : connection attempt failed.\n"); close(fd_client_socket); }
				}
				else
				{
					memset(buffer, 0, BUFFER_SIZE);
					recv_read = recv(poll_fds[i].fd, buffer, (BUFFER_SIZE - 1), 0);
					if (recv_read == 0)
					{
						printf("Client on fd %d disconnected.\n", poll_fds[i].fd);
						close(poll_fds[i].fd);
						poll_fds[i] = poll_fds[poll_fd_nb - 1];
						poll_fd_nb--;
						i--;
					}
					else
					{
						if (recv_read < BUFFER_SIZE) { buffer[recv_read] = '\0'; }
						else { buffer[BUFFER_SIZE - 1] = '\0'; }
						printf("Received from fd %d: %s", poll_fds[i].fd, buffer);
					}
				}
			}
		}
	}
	close(fd_server_socket);
	return 0;
}
