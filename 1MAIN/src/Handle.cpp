//
// Created by nidionis on 02/07/25.
//

#include <Handle.hpp>
#include <Channel.hpp>
#include <iostream>
#include <vector>

#include "../include/Channel.hpp"
#include "../include/Server.hpp"
#include "../include/utils_strings.hpp"

void cmdCap(Server& server, Client& client, std::string args)
{
    (void)server;
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
}

void cmdNick(Server& server, Client& client, std::string input)
{
    (void)server;
    std::string nick = getHead(input);
    if (nick != "")
    {
        if (server.hasNick(nick))
            client.send("NICK :NickName already in use\r\n");
        else
            client.setNickname(nick);
    }
    else
        client.send("NICK :You are now known as " + client.getNickname() + "\r\n");
}

void cmdUser(Server& server, Client& client, std::string input)
{
    (void)server;
    std::string user = getHead(input);
    std::string realname = lastWord(input);
    if (client.getUsername() != "") {
        throw std::runtime_error("USER :You are already logged in");
    } if (server.hasUser(user))
    {
        throw std::runtime_error("USER :Username already in use\r\n");
    }
    client.setUsername(user);
    client.setRealname(realname);
}

void cmdJoin(Server& server, Client& client, std::string input)
{
    (void)server;
    (void)client;
    std::string channel_str = popWd(input);
    std::string key = lastWord(input);
    Channel channel;
    if (client.hasFlag(LOGGED) == false)
    {
        throw (std::runtime_error("Client not logged in"));
    }
    if (channel_str[0] == '#' && isValidName(channel_str.substr(1)))
    {
        try
        {
            channel = server.getChannel(channel_str);
            try {
                if (channel.getKey() != "" && channel.getKey() != key) {
                    throw std::runtime_error("JOIN :Invalid channel key\r\n");
                }
                channel.setClient(client);
            }
            catch (std::runtime_error& err)
            {
                client.send("JOIN :You are already in the channel\r\n");
            }
        }
        catch (const std::runtime_error& err)
        {
            client.send("JOIN :Channel does not exist\r\n");
            try
            {
                client.newChannel(channel_str);
            }
            catch (std::runtime_error& err)
            {
                client.send("JOIN : failed :Too many channels\r\n");
            }
        }
    }
    else
    {
        client.send("JOIN :Invalid channel name\r\n");
    }
}

void cmdMode(Server& server, Client& client, std::string input)
{
    (void)server;
    (void)client;
    (void)input;
    std::string item = popWd(input);
    std::string mode_chars = popWd(input);
    std::string args = input;
    Channel channel;
    if (client.hasFlag(LOGGED) == false) {
        throw (std::runtime_error("Client not logged in\r\n"));
    }
    if (item[0] == '#') {
        try {
            channel = server.getChannel(item);
        } catch (const std::runtime_error& err) {
            throw std::runtime_error(err.what());
        }
        if (channel.isOperator(client))
        {
            if (mode_chars[0] == '+' || mode_chars[0] == '-') {
                if (!strchr(AVAILABLE_MODE, mode_chars[1])) {
                    throw std::runtime_error("MODE :Invalid mode\r\n");
                }
            }
            if (mode_chars[0] == '+') {
                if (mode_chars[1] == 'k') {
                    channel.setKey(args);
                } else {
                    channel.setFlag(mode_chars[1]);
                }
            } else if (mode_chars[0] == '-') {
                if (mode_chars[1] == 'k') {
                    channel.setKey("");
                } else {
                    channel.delFlag(mode_chars[1]);
                }
            }
        }
    }
    else
    {
        if (server.hasUser(item))
        {
            client.send("[debug] do something with user here\r\n");
        }
    }
}

void cmdKick(Server& server, Client& client, std::string input)
{
    (void)server;
    (void)client;
    std::string channel_str = popWd(input);
    Client& kicked = server.getClient(getHead(input));
    std::string reason = getNextWds(input);
    if (channel_str[0] == '#' && isValidName(channel_str.substr(1)))
    {
        try
        {
            Channel& channel = server.getChannel(channel_str);
            if (channel.isOperator(client))
            {
                try
                {
                    channel.delClient(client);
                }
                catch (std::runtime_error& err)
                {
                    client.send("JOIN : Nick ");
                    client.send(kicked.getNickname());
                    client.send("not in the channel\r\n");
                }
            }
            else
                client.send("JOIN :You are not an operator of the channel\r\n");
        }
        catch (const std::runtime_error& err)
        {
            client.send("JOIN :Channel does not exist\r\n");
        }
    }
}

void cmdTopic(Server& server, Client& client, std::string input)
{
    (void)server;
    std::string channel_str = getHead(input);
    std::string topic = getNextWds(input);
    if (client.hasFlag(LOGGED) == false)
    {
        throw (std::runtime_error("Client not logged in"));
    }
    if (channel_str[0] == '#' && isValidName(channel_str.substr(1)))
    {
        try
        {
            Channel& channel = server.getChannel(channel_str);
            if (channel.isOperator(client))
            {
                channel.setTopic(topic);
            }
        }
        catch (const std::runtime_error& err)
        {
            client.send("TOPIC : ");
            client.send(err.what());
            client.send("\r\n");
        }
    }
}

void cmdPing(Server& server, Client& client, std::string input)
{
    (void)server;

    client.send(":");
    client.send(server.getName() + " PONG :");
    client.send(input);
    client.send("\r\n");
}

void cmdWho(Server& server, Client& client, std::string input)
{
    (void)server;
    (void)input;
    std::string message352 = ":ircSchoolProject 352 " + client.getNickname() + " * ~" + client.getUsername()
        + " 10.13.4.10 ircSchoolProject " + client.getNickname() + " H :0 " + client.getRealname() + '\n';
    std::string message315 = ":ircSchoolProject 315 " + client.getNickname()
        + " " + client.getNickname() + " :End of /WHO list.\n";
    client.send(message352);
    client.send(message315);
}

void cmdUserHost(Server& server, Client& client, std::string input)
{
    (void)server;
    std::string arg = getHead(input);
    if (arg == client.getNickname())
    {
        std::string message302 = ":ircSchoolProject 302 " + client.getNickname()
           + " :" + client.getNickname() + "=+~" + client.getUsername() + "@10.13.4.10\n";
        client.send(message302);
    }
}

void cmdPass(Server& server, Client& client, std::string input)
{
    (void)server;
    std::string passwd = trim(input);
    if (server.checkPasswd(passwd))
    {
        client.setFlag(PASSWD_OK);
        std::cout << "passwd validated" << std::endl;
        std::cout << "\r\n" << std::endl;
    }
    else
    {
        throw std::runtime_error("PASS :Password incorrect\r\n");
    }
}

void cmdPrivmsg(Server& server, Client& client, std::string input)
{
    (void)server;
    std::string name = getHead(input);
    name = trim(name, OPERATOR_OP);
    std::string msg = getNextWds(input);
    if (client.hasFlag(LOGGED) == false)
    {
        throw (std::runtime_error("Client not logged in"));
    }
    if (name[0] == '#' && isValidName(name.substr(1)))
    {
        try
        {
            Channel& channel = server.getChannel(name);
            channel.spawn(client.getNickname() + " : " + msg);
        }
        catch (const std::runtime_error& err)
        {
            client.send(err.what());
        }
    }
    else if (server.hasNick(name))
    {
        Client dest = server.getClient(name);
        dest.send(client.getNickname() + " : " + msg);
        dest.send("\r\n");
    }
    else
    {
        throw std::runtime_error("PRIVMSG : channel or client not found");
    }
    throw std::runtime_error("PRIVMSG : Failure");
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
                client.send("\n");
            }
            return;
        }
    }
    client.send("[processCommand]: Invalid command");
    client.send(input);
    std::cout << "<<<<<<<<<<<<<<<<<<" << std::endl;
}

void cmdInvite(Server &server, Client &client, std::string input) {
    (void)server;
    Client dest;
    Channel channel;
    try {
        dest = server.getClient(popWd(input));
    } catch (std::runtime_error& err) {
        throw std::runtime_error(err.what());
    }
    try {
        channel = server.getChannel(popWd(input));
    } catch (std::runtime_error& err) {
        throw std::runtime_error(err.what());
    }

    if (channel.hasFlag('i')) {
        if (channel.isOperator(client)) {
            throw std::runtime_error(client.getNickname() + channel.getName() + "you are not operrator");
        }
    }
    dest.send(client.getNickname() + " INVITE " + dest.getNickname() + " " + channel.getName() + "\r\n");
    channel.setClient(dest);
}
