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
# include <sstream>
# include <algorithm>
# include <cstring>
# include <cstdlib>
# include <cerrno>
# include <csignal>
# include <exception>

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <poll.h>
# include <errno.h>
# include <fcntl.h>
# include <utils_strings.hpp>
# include <capabilities.hpp>
# include <vector_utils.hpp>

#define GETTER_SETTER(type, name, f_name) \
type get##f_name() const { return name; } \
void set##f_name(type value) { name = value; }

# define BUFFER_SIZE 1024
# define PORT 6667
# define QUEUE_SIZE 16
# define MAX_CLIENTS 3
# define MAX_CONNECTIONS (MAX_CLIENTS + 1)

class Server;
class Client;
class Channel;

struct poll_data
{
	struct pollfd		fds[MAX_CONNECTIONS];
	int					fd_nb;
	int					i;
	int					err_check;
};

void	pollDataCleanup(poll_data* poll_data);
bool	cmpHead(const std::string str1, const std::string str2);

# include <Handle.hpp>
# include <Channel.hpp>
# include <Server.hpp>
# include <Client.hpp>

#endif // MAIN_HPP
