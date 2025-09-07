/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 12:24:25 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/31 12:40:09 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by nidionis on 02/07/25.
//

#include "handle.hpp"
#include "irc_numeric.hpp"

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
	{
		// ERR_NEEDMOREPARAMS (461) CAP :Not enough parameters
		throw std::runtime_error(
			makeNumeric(server, client, ERR_NEEDMOREPARAMS, {"CAP"}, "Not enough parameters")
		);
	}
	if (getHead(args) == "LS")
		{ capLs(server, client, getNextWds(args)); }
	else if (getHead(args) == "REQ")
		{ capReq(server, client, getNextWds(args)); }
	else if (getHead(args) == "END")
		{ capEnd(server, client, getNextWds(args)); }
	else
	{
		// Use ERR_UNKNOWNCOMMAND (421) for invalid CAP subcommand
		throw std::runtime_error(
			makeNumeric(server, client, ERR_UNKNOWNCOMMAND, {"CAP"}, "Invalid CAP subcommand")
		);
	}
	return ;
}

void	cmdNick(Server& server, Client& client, std::string args)
{
	std::string	nick = getHead(args);

	if (!client.hasFlag(PASSWD_OK))
		{ client.setmust_kill(true); return ; }
	if (nick == "")
	{
		// ERR_NONICKNAMEGIVEN (431) :No nickname given
		throw std::runtime_error(
			makeNumeric(server, client, ERR_NONICKNAMEGIVEN, {}, "No nickname given")
		);
	}
	if (server.clientHasNick(nick))
	{
		// ERR_NICKNAMEINUSE (433) * <nick> :Nickname is already in use.
		throw std::runtime_error(
			makeNumeric(server, std::string("*"), ERR_NICKNAMEINUSE, {nick}, "Nickname is already in use.")
		);
	}
	if (client.getNickname() != "")
		{ client.send( ":" + client.getNickname() + "!~" + client.getUsername() + "@" + client.getIp() + " NICK :" + nick + "\r\n"); }
	client.setNickname(nick);
	if (client.isLogged()) { server_banner(client, server); }
	return ;
}

void	cmdUser(Server& server, Client& client, std::string args)
{
	std::string	user = getHead(args);
	std::string	realname = lastWord(args);

	if (!client.hasFlag(PASSWD_OK))
	{
		client.setmust_kill(true);
		return ;
	}
	if (client.getUsername() != "")
	{
		// ERR_ALREADYREGISTRED (462) :You may not reregister
		throw std::runtime_error(
			makeNumeric(server, client, ERR_ALREADYREGISTRED, {}, "You may not reregister")
		);
	}
	client.setUsername(user);
	client.setRealname(realname);
	std::cout << "TEST USER : [" << client.getUsername() << "]" << std::endl;
}

void	cmdPing(Server& server, Client& client, std::string args)
{
	client.send(":" + server.getName() + " PONG :" + args + "\r\n");
	return ;
}

/*
Command: WHO
Parameters: <mask>

// user requested but does not exist
[14:16] << WHO NotAName%0A
[14:16] >> :anarchy.esper.net 315 NickName NotAName :End of /WHO list.%0A

// user requested, exists, and is not part of any channel
[14:17] << WHO NickName%0A
[14:17] >> :anarchy.esper.net 352 NickName * ~UserName 45.148.156.203 anarchy.esper.net NickName H :0 RealName%0A
[14:17] >> :anarchy.esper.net 315 NickName NickName :End of /WHO list.%0A

// user requested but not present in any channel
[13:18] << WHO NickName%0A
[13:18] >> :calamity.esper.net 352 NickName * ~UserName 45.148.156.203 calamity.esper.net NickName H :0 RealName%0A
[13:18] >> :calamity.esper.net 315 NickName NickName :End of /WHO list.%0A

// user requested and is present in at least one channel (display first channel found)
[14:18] << WHO NickName%0A
[14:18] >> :anarchy.esper.net 352 NickName #test984 ~UserName 45.148.156.203 anarchy.esper.net NickName H@ :0 RealName%0A
[14:18] >> :anarchy.esper.net 315 NickName NickName :End of /WHO list.%0A

// channel requested but does not exist OR user not part of it
[14:20] << WHO #nochannel%0A
[14:20] >> :anarchy.esper.net 315 NickName #nochannel :End of /WHO list.%0A

// channel requested, exists AND user is part of it, list all users inside
[15:43] << WHO #test123%0A
[15:43] >> :calamity.esper.net 352 NickName #test123 ~lahlsweh 45.148.156.203 calamity.esper.net test1 H :0 Lucas Ahlsweh%0A
[15:43] >> :calamity.esper.net 352 NickName #test123 ~lahlsweh 45.148.156.203 calamity.esper.net test2 H :0 Lucas Ahlsweh%0A
[15:43] >> :calamity.esper.net 352 NickName #test123 ~UserName 45.148.156.203 calamity.esper.net NickName H@ :0 RealName%0A
[15:43] >> :calamity.esper.net 315 NickName #test123 :End of /WHO list.%0A
*/

void cmdWho(Server& server, Client& client, std::string args)
{
	std::string	name = getHead(args);
	std::string	message352 = ":" + server.getName() + " 352 " + client.getNickname() + " * ~" + client.getUsername() + " "
		+ getLocalIPv4Address() + " " + server.getName() + " " + client.getNickname() + " H :0 " + client.getRealname() + "\r\n";
	std::string	message315 = ":" + server.getName() + " 315 " + client.getNickname() + " " + client.getNickname() + " :End of /WHO list.\r\n";
	
	if (name != client.getNickname())
	{
		std::cerr << "/WHO Warning : Feature not implemented" << std::endl;
		return ;
	}
	std::cerr << "/WHO Warning : Only default behaviour has been implemented" << std::endl;
	client.send(message352);
	client.send(message315);
	return ;
}

void	cmdUserHost(Server& server, Client& client, std::string args)
{
	std::string arg = getHead(args);
	if (arg == client.getNickname())
	{
		// RPL_USERHOST (302) :<nick>=+~<user>@<host>
		sendNumeric(server, client, RPL_USERHOST, {},
		            client.getNickname() + "=+~" + client.getUsername() + "@" + server.getIp());
	}
}

void	cmdPass(Server& server, Client& client, std::string args)
{
	std::string	passwd = trim(args);

	if (client.hasFlag(PASSWD_OK))
	{
		// ERR_ALREADYREGISTRED (462) :You may not reregister
		throw std::runtime_error(
			makeNumeric(server, client, ERR_ALREADYREGISTRED, {}, "You may not reregister")
		);
	}
	if (passwd == "")
	{
		// ERR_NEEDMOREPARAMS (461) PASS :Not enough parameters
		throw std::runtime_error(
			makeNumeric(server, client, ERR_NEEDMOREPARAMS, {"PASS"}, "Not enough parameters")
		);
	}
	if (server.checkPasswd(passwd))
	{
		client.setFlag(PASSWD_OK);
		std::cout << "passwd validated\r" << std::endl;
	}
	else
	{
		// ERR_PASSWDMISMATCH (464) :Password incorrect
		client.send(makeNumeric(server, client, ERR_PASSWDMISMATCH, {}, "Password incorrect"));
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
				// Avoid double CRLF and also handle legacy errors without CRLF
				if (str_err.size() >= 2 && str_err.substr(str_err.size() - 2) == "\r\n")
					client.send(str_err);
				else
					client.send(str_err + "\r\n");
				if (client.getmust_kill() == true) { return ; }
			}
			return ;
		}
	}
	// ERR_UNKNOWNCOMMAND (421) <command> :Unknown command
	client.send(makeNumeric(server, client, ERR_UNKNOWNCOMMAND, {cmd_flg}, "Unknown command"));
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
