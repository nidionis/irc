/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:48:10 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/07 12:45:00 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HPP
# define MAIN_HPP

# include <algorithm>

# include <poll.h>

# define MAX_CLIENTS 3
# define MAX_CONNECTIONS (MAX_CLIENTS + 1)

struct poll_data
{
	struct pollfd		fds[MAX_CONNECTIONS];
	int					fd_nb;
	int					i;
	int					err_check;
};

# include "Client.hpp"
# include "utils_strings.hpp"
# include "vector_utils.hpp"

void	pollDataCleanup(poll_data* poll_data);

#endif // MAIN_HPP
