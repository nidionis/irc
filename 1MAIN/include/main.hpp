/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:48:10 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/30 13:39:57 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HPP
# define MAIN_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <cstdlib>
# include <cerrno>
# include <csignal>
# include <exception>
#include <vector>

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <poll.h>
# include <errno.h>
# include <fcntl.h>

# define BUFFER_SIZE 1024
# define PORT 6667
# define QUEUE_SIZE 16
# define MAX_CLIENTS 3
# define MAX_CONNECTIONS (MAX_CLIENTS + 1)

struct poll_data
{
	struct pollfd		fds[MAX_CONNECTIONS];
	int					fd_i;
	int					i;
	int					err_check;
};

void	pollDataCleanup(poll_data* poll_data);

#endif // MAIN_HPP
