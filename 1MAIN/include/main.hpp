/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:48:10 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/27 15:49:02 by lahlsweh         ###   ########.fr       */
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
//# include <netinet/in.h>
//# include <netdb.h>
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
//# define ERROR -1
//# define OK 0
# define PORT 6667
//# define MAX_SOCKET 3
# define QUEUE_SIZE 16
# define MAX_CLIENTS 3
# define MAX_CONNECTIONS (MAX_CLIENTS + 1)
# define FAIL -1

#endif // MAIN_HPP
