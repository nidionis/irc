/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:43:51 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/07 16:07:12 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by nidionis on 02/07/25.
//

#include "handle.hpp"

void cmdDebug(Server& server, Client& client, std::string args)
{
    (void)client;
    (void)args;
    std::cout << "serv port ";
    std::cout << server.getPort();
}

void cmdCap(Server& server, Client& client, std::string args)
{
    if (getHead(args) == "")
    {
        throw (std::runtime_error(":" + server.getName() + " 461 " + client.getNickname() + " CAP :Not enough parameters\r\n"));
    }
    if (getHead(args) == "LS")
    {
        capLs(server, client, getNextWds(args));
    }
    else if (getHead(args) == "REQ")
    {
        capReq(server, client, getNextWds(args));
    }
    else if (getHead(args) == "END")
    {
        capEnd(server, client, getNextWds(args));
    }
    else
    {
        throw (std::runtime_error(":" + server.getName() + " 410 " + client.getNickname() + getHead(args) + " :Invalid CAP subcommand\r\n"));
    }
}

void cmdNick(Server& server, Client& client, std::string input)
{
    std::string nick = getHead(input);

    if (!client.hasFlag(PASSWD_OK))
    {
        client.must_kill = true;
        return ;
    }
    if (nick == "")
        throw (std::runtime_error(":" + server.getName() + " 431 " + client.getNickname() + " : No nickname given\r\n"));

    if (server.hasNick(nick))
        throw (std::runtime_error (":" + server.getName() + " 433 * " + nick + " :Nickname is already in use.\r\n"));

    if (client.getNickname() != "")
        client.send( ":" + client.getNickname() + "!~" + client.getUsername() + "@" + client.getIp() + " NICK :" + nick + "\r\n");
    client.setNickname(nick);

    if (client.isLogged())
        server_banner(client, server);
}

void cmdUser(Server& server, Client& client, std::string input)
{
    (void)server;
    std::string user = getHead(input);
    std::string realname = lastWord(input);

    if (!client.hasFlag(PASSWD_OK))
    {
        client.must_kill = true;
        return ;
    }
    if (client.getUsername() != "")
    {
        std::string message462 = client.getNickname() + " :You may not reregister\r\n";
        server.sendHead(client, "462");
        client.send(message462);
        throw std::runtime_error("");
    }
    client.setUsername(user);
    client.setRealname(realname);
    std::cout << "TEST USER : [" << client.getUsername() << "]" << std::endl;
}   


void cmdPing(Server& server, Client& client, std::string input)
{
    client.send(":");
    client.send(server.getName() + " PONG :");
    client.send(input);
    client.send("\r\n");
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

void cmdWho(Server& server, Client& client, std::string input)
{
    (void)input;
    std::string name = getHead(input);
    server.sendHead(client, "352");
    Channel channel;
    if (server.hasNick(name)) {
        Client target = server.getClient(name);
        channel = server.getChannel(target.getChannel().getName());
    } else if (server.hasChannel(name)) {
        channel = server.getChannel(name);
    }
    client.send(channel.getName());
    client.send(" ");
    client.send(getLocalIPv4Address());
    client.send(server.getName() + " H@ :0 " + client.getRealname() + "\r\n");
    server.sendHead(client, "312");
    client.send(client.getNickname() + ":End of /WHO list.\r\n");
}

void cmdUserHost(Server& server, Client& client, std::string input)
{
    std::string arg = getHead(input);
    if (arg == client.getNickname())
    {
        std::string message302 = ":" + client.getNickname() + "=+~" + client.getUsername() + "@" + server.getIp() + "\n";
        server.sendHead(client, "302");
        client.send(message302);
    }
}

void cmdPass(Server& server, Client& client, std::string input)
{
    std::string passwd = trim(input);
    if (client.hasFlag(PASSWD_OK))
    {
        throw (std::runtime_error(":" + server.getName() + " 462 " + client.getNickname() + " PASS :You may not reregister\r\n"));
    }
    if (passwd == "")
    {
        throw (std::runtime_error(":" + server.getName() + " 461 " + client.getNickname() + " PASS :Not enough parameters\r\n"));
    }
    if (server.checkPasswd(passwd))
    {
        client.setFlag(PASSWD_OK);
        std::cout << "passwd validated" << std::endl;
        std::cout << "\r\n" << std::endl;
    }
    else
    {
        client.send(":" + server.getName() + " 464 " + client.getNickname() + " PASS :Password incorrect\r\n");
        client.must_kill = true;
        return ;
    }
}


void processCommand(Server& server, Client& client, std::string input)
{
    std::cout << "##############################" << std::endl;
    std::cout << "<< " << input << std::endl;
    std::string cmd_flg = upperCase(getHead(input));
    std::string cmd_arg = getNextWds(input);
    for (int i = 0; commands[i].f != NULL; i++)
    {
        if (cmd_flg == commands[i].header)
        {
            try
            {
                commands[i].f(server, client, cmd_arg);
            }
            catch (std::runtime_error& err)
            {
                client.send(err.what());
                client.send("\r\n");
                if (client.must_kill == true) { return ; }
            }
            return ;
        }
    }
    client.send("[processCommand]: Invalid command");
    client.send(input);
    std::cout << "<<<<<<<<<<<<<<<<<<" << std::endl;
}

void cmdQuit(Server &server, Client &client, std::string input)
{
    (void)server;
    (void)input;
    std::cout << "TMP cmdQuit()" << std::endl;
    client.must_kill = true;
    return ;
}
