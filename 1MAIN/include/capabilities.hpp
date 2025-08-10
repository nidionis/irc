/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capabilities.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:43:27 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/10 11:24:19 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by nidionis on 20/07/25.
//

#ifndef CAPABILITIES_HPP
# define CAPABILITIES_HPP


# include "handle.hpp"

class Client;

void	multiPrefix(Server &server, Client &client, std::string args);
void	capLs(Server &server, Client &client, std::string args);
void	capReq(Server &server, Client &client, std::string args);
void	capEnd(Server &server, Client &client, std::string args);
bool	isCap(const std::string& cap);
void	server_banner(Client &client, Server &server);

typedef void(*CommandFunc2)(Server &server, Client &client, std::string args);

struct	s_cmd2
{
	const char		*header;
	CommandFunc2	f;
};

static const struct s_cmd2	cap_tab[] =
{
	{"multi-prefix",	&multiPrefix},
	{"",				NULL}
};

#endif //CAPABILITIES_HPP
