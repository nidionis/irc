/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:20:36 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/10 11:42:37 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by nidionis on 02/07/25.
//

#include "handle.hpp"

void	cmdDebug(Server &server, Client &client, std::string args)
{
	(void)client;
	(void)args;
	std::cout << "serv port " << server.getPort() << std::endl;
	return ;
}

void	cmdCap(Server &server, Client &client, std::string args)
{
	if (getHead(args) == "")
		{ throw (std::runtime_error(":" + server.getName() + " 461 " + client.getNickname() + " CAP :Not enough parameters\r\n")); }
	if (getHead(args) == "LS")
		{ capLs(server, client, getNextWds(args)); }
	else if (getHead(args) == "REQ")
		{ capReq(server, client, getNextWds(args)); }
	else if (getHead(args) == "END")
		{ capEnd(server, client, getNextWds(args)); }
	else
		{ throw (std::runtime_error(":" + server.getName() + " 410 " + client.getNickname() + getHead(args) + " :Invalid CAP subcommand\r\n")); }
	return ;
}

void	cmdNick(Server &server, Client &client, std::string args)
{
	std::string nick = getHead(args);

	if (!client.hasFlag(PASSWD_OK))
		{ client.setmust_kill(true); return ; }
	if (nick == "")
		{ throw (std::runtime_error(":" + server.getName() + " 431 " + client.getNickname() + " : No nickname given\r\n")); }
	if (server.hasNick(nick))
		{ throw (std::runtime_error (":" + server.getName() + " 433 * " + nick + " :Nickname is already in use.\r\n")); }
	if (client.getNickname() != "")
		{ client.send( ":" + client.getNickname() + "!~" + client.getUsername() + "@" + client.getIp() + " NICK :" + nick + "\r\n"); }
	client.setNickname(nick);
	if (client.isLogged()) { server_banner(client, server); }
	return ;
}

void	cmdUser(Server &server, Client &client, std::string args)
{
	(void)server;
	std::string	user = getHead(args);
	std::string	realname = lastWord(args);

	if (!client.hasFlag(PASSWD_OK))
		{ client.setmust_kill(true); return ; }
	if (client.getUsername() != "")
	{
		std::string message462 = ":ircSchoolProject 462 " + client.getNickname() + " :You may not reregister\r\n";
		throw (std::runtime_error(message462));
	}
	client.setUsername(user);
	client.setRealname(realname);
	return ;
}

void	cmdPing(Server &server, Client &client, std::string args)
{
	std::string	message_pong = ":" + server.getName() + " PONG :" + args + "\r\n";

	client.send(message_pong);
	return ;
}

/*
Command: WHO
Parameters: <mask>

// if user is requested and present in several channels, only first channel found is displayed
[15:40] << WHO NickName%0A
[15:40] >> :calamity.esper.net 352 NickName #test789 ~UserName 45.148.156.203 calamity.esper.net NickName H@ :0 RealName%0A
[15:40] >> :calamity.esper.net 315 NickName NickName :End of /WHO list.%0A

// if channel is requested, list all users inside
[15:43] << WHO #test123%0A
[15:43] >> :calamity.esper.net 352 NickName #test123 ~lahlsweh 45.148.156.203 calamity.esper.net test1 H :0 Lucas Ahlsweh%0A
[15:43] >> :calamity.esper.net 352 NickName #test123 ~lahlsweh 45.148.156.203 calamity.esper.net test2 H :0 Lucas Ahlsweh%0A
[15:43] >> :calamity.esper.net 352 NickName #test123 ~UserName 45.148.156.203 calamity.esper.net NickName H@ :0 RealName%0A
[15:43] >> :calamity.esper.net 315 NickName #test123 :End of /WHO list.%0A
*/
void	cmdWho(Server &server, Client &client, std::string args)
{
	(void)args;
	std::string	message352 = ":ircSchoolProject 352 " + client.getNickname() + " * ~" + client.getUsername()
		+ " " + server.getIp() + " ircSchoolProject " + client.getNickname() + " " + client.getRealname() + '\n';
	std::string message315 = ":ircSchoolProject 315 " + client.getNickname()
		+ " " +	client.getNickname() + " :End of /WHO list.\n";

	client.send(message352);
	client.send(message315);
	return ;
}

void	cmdUserHost(Server &server, Client &client, std::string args)
{
	std::string	arg = getHead(args);
	if (arg == client.getNickname())
	{
		std::string	message302 = ":ircSchoolProject 302 " + client.getNickname()
			+ " :" + client.getNickname() + "=+~" + client.getUsername() + "@" + server.getIp() + "\n";
		client.send(message302);
	}
	return ;
}

void	cmdPass(Server &server, Client &client, std::string args)
{
	std::string	passwd = trim(args);

	if (client.hasFlag(PASSWD_OK))
		{ throw (std::runtime_error(":" + server.getName() + " 462 " + client.getNickname() + " PASS :You may not reregister\r\n")); }
	if (passwd == "")
		{ throw (std::runtime_error(":" + server.getName() + " 461 " + client.getNickname() + " PASS :Not enough parameters\r\n")); }
	if (server.checkPasswd(passwd))
	{
		client.setFlag(PASSWD_OK);
		std::cout << "passwd validated\r" << std::endl;
	}
	else
	{
		client.send(":" + server.getName() + " 464 " + client.getNickname() + " PASS :Password incorrect\r\n");
		client.setmust_kill(true);
		return ;
	}
}


void	processCommand(Server &server, Client &client, std::string args)
{
	std::string	cmd_flg = upperCase(getHead(args));
	std::string	cmd_arg = getNextWds(args);

	std::cout << "##############################" << std::endl;
	std::cout << "<< " << args << std::endl;
	for (int i = 0; commands[i].f != NULL; i++)
	{
		if (cmd_flg == commands[i].header)
		{
			try { commands[i].f(server, client, cmd_arg); }
			catch (std::runtime_error& err)
			{
				client.send(err.what());
				client.send("\r\n");
				if (client.getmust_kill() == true) { return ; }
			}
			return ;
		}
	}
	client.send("[processCommand]: Invalid command");
	client.send(args);
	std::cout << "<<<<<<<<<<<<<<<<<<" << std::endl;
	return ;
}

void	cmdQuit(Server &server, Client &client, std::string args)
{
	(void)server;
	(void)args;
	std::cout << "TMP cmdQuit()" << std::endl;
	client.setmust_kill(true);
	return ;
}
