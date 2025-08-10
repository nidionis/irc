/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capabilities.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:20:11 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/10 13:15:46 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by n on 20/07/25.
//

#include "capabilities.hpp"

bool	isCap(const std::string& cap)
{
	if (cap.empty()) { return false; }
	for (size_t i = 0; cap_tab[i].header[0] != '\0'; ++i)
		{ if (cap == cap_tab[i].header) { return (true); } }
	return (false);
}

void	multiPrefix(Server& server, Client& client, std::string args)
{
	(void)server;
	(void)args;

	client.setCap("multi-prefix");
	return ;
}

void	capLs(Server& server, Client& client, std::string args)
{
	(void)args;
	std::string	msg_cap_ls = ":" + server.getName() + " CAP * LS :";

	for (int i = 0; cap_tab[i].f; i++)
		{ msg_cap_ls += cap_tab[i].header; }
	msg_cap_ls += "\r\n";
	client.send(msg_cap_ls);
	return ;
}


void	capReq(Server& server, Client& client, std::string args)
{
	std::cout << "[capReq] caps:" << args << std::flush;
	std::string	cap = popWd(args);
	std::string	messageCAP_ACK;
	if (!cap.empty())
	{
		for (int i = 0; cap_tab[i].f; i++)
		{
			if (cap == cap_tab[i].header)
			{
				cap_tab[i].f(server, client, "");
				if (client.getNickname() != "")
					{ messageCAP_ACK = ":" + server.getName() + " CAP " + client.getNickname() + " ACK :" + cap + "\r\n"; }
				else 
					{ messageCAP_ACK = ":" + server.getName() + " CAP " + "*" + " ACK :" + cap + "\r\n"; }
				client.send(messageCAP_ACK);
			}
			else 
				{ client.send(":" + server.getName() + " CAP " + client.getNickname() + " NAK :" + cap + "\r\n"); }
		}
	}
	return ;
}

void	server_banner(Client &client, Server &server)
{
	if (client.hasFlag("BANNER_SENT")) { return ; }
	client.send(":ircSchoolProject 001 " + client.getNickname() + " :Welcome to the ircSchoolProject " + client.getNickname() + "\r\n");
	client.send(":ircSchoolProject 002 " + client.getNickname() + " :Your host is ircSchoolProject[" + server.getIp() + "/" + ft_put_uint16(server.getPort()) + "], running version v1.0\r\n");
	client.send(":ircSchoolProject 003 " + client.getNickname() + " :This server was created Wed Jul 2025 at 12:00:00 EST\r\n");
	client.send(":ircSchoolProject 004 " + client.getNickname() + " ircSchoolProject v1.0 o itkol kl\r\n");
	client.send(":ircSchoolProject 005 " + client.getNickname() + " CHANMODES=o,k,l,it :are supported by this server\r\n");
	client.send(":" + client.getNickname() + " MODE " + client.getNickname() + " :+i\r\n");
	client.setFlag("BANNER_SENT");
	return ;
}

void	capEnd(Server &server, Client &client, std::string args)
{
	(void)args;

	client.setFlag("CAP_END");
	if (client.isLogged()) { server_banner(client, server); }
	return ;
}
