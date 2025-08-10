/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:20:36 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/10 15:05:02 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by nidionis on 02/07/25.
//

#include "handle.hpp"

void	cmdDebug(Server& server, Client& client, std::string args)
{
	(void)client;
	(void)args;

	std::cout << "serv port " << server.getPort() << std::endl;
	return ;
}

void	cmdCap(Server& server, Client& client, std::string args)
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

void	cmdNick(Server& server, Client& client, std::string args)
{
	std::string	nick = getHead(args);

	if (!client.hasFlag(PASSWD_OK))
		{ client.setmust_kill(true); return ; }
	if (nick == "")
		{ throw (std::runtime_error(":" + server.getName() + " 431 " + client.getNickname() + " : No nickname given\r\n")); }
	if (server.clientHasNick(nick))
		{ throw (std::runtime_error (":" + server.getName() + " 433 * " + nick + " :Nickname is already in use.\r\n")); }
	if (client.getNickname() != "")
		{ client.send( ":" + client.getNickname() + "!~" + client.getUsername() + "@" + client.getIp() + " NICK :" + nick + "\r\n"); }
	client.setNickname(nick);
	if (client.isLogged()) { server_banner(client, server); }
	return ;
}

void	cmdUser(Server& server, Client& client, std::string args)
{
	(void)server;
	std::string	user = getHead(args);
	std::string	realname = lastWord(args);

	if (!client.hasFlag(PASSWD_OK))
		{ client.setmust_kill(true); return ; }
	if (client.getUsername() != "")
		{ throw (std::runtime_error(":ircSchoolProject 462 " + client.getNickname() + " :You may not reregister\r\n")); }
	client.setUsername(user);
	client.setRealname(realname);
	return ;
}

void	cmdPing(Server& server, Client& client, std::string args)
{
	client.send(":" + server.getName() + " PONG :" + args + "\r\n");
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
void	cmdWho(Server& server, Client& client, std::string args)
{
	(void)args;

	client.send(":ircSchoolProject 352 " + client.getNickname() + " * ~" + client.getUsername() + " " + server.getIp() + " ircSchoolProject " + client.getNickname() + " " + client.getRealname() + "\r\n");
	client.send(":ircSchoolProject 315 " + client.getNickname() + " " +	client.getNickname() + " :End of /WHO list.\r\n");
	return ;
}

void	cmdUserHost(Server& server, Client& client, std::string args)
{
	std::string	arg = getHead(args);

	if (arg == client.getNickname())
		{ client.send(":ircSchoolProject 302 " + client.getNickname() + " :" + client.getNickname() + "=+~" + client.getUsername() + "@" + server.getIp() + "\r\n"); }
	return ;
}

void	cmdPass(Server& server, Client& client, std::string args)
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


void	processCommand(Server& server, Client& client, std::string args)
{
	std::string	cmd_flg = upperCase(getHead(args));
	std::string	cmd_arg = getNextWds(args);
	std::string	str_err;

	std::cout << "##############################" << std::endl
			  << "<< " << args << std::flush;
	for (int i = 0; commands[i].f != NULL; i++)
	{
		if (cmd_flg == commands[i].header)
		{
			try { commands[i].f(server, client, cmd_arg); }
			catch (std::runtime_error& err)
			{
				str_err = err.what();
				client.send(str_err + "\r\n");
				if (client.getmust_kill() == true) { return ; }
			}
			return ;
		}
	}
	client.send(":" + server.getName() + " 421 " + client.getNickname() + ' ' + cmd_flg + " :Unknown command\r\n");
	std::cout << "<<<<<<<<<<<<<<<<<<" << std::endl;
	return ;
}

void	cmdQuit(Server& server, Client& client, std::string args)
{
	(void)server;
	(void)args;

	std::cout << "TMP cmdQuit()" << std::endl;
	client.setmust_kill(true);
	return ;
}
