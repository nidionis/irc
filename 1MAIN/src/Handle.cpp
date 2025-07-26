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
    if (user != "" && realname != "")
    {
        if (client.getUsername() != "")
        {
            throw std::runtime_error("USER :You are already logged in");
        }
        if (server.hasUser(user))
            client.send("USER :Username already in use\r\n");
        else
        {
            client.setUsername(user);
            client.setRealname(realname);
        }
    } else {
        throw std::runtime_error("USER :failed to set names");
    }
}

void cmdJoin(Server& server, Client& client, std::string input)
{
    (void)server;
    (void)client;
    if (client.hasFlag(LOGGED) == false)
    {
        throw (std::runtime_error("Client not logged in"));
    }
    std::string channel_str = getHead(input);
    Channel channel;
    if (channel_str[0] == '#' && isValidName(channel_str.substr(1)))
    {
        try
        {
            channel = server.getChannel(channel_str);
            try
            {
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
    if (client.hasFlag(LOGGED) == false)
    {
        throw (std::runtime_error("Client not logged in"));
    }
    std::string item = getHead(input);
    std::string mode_char = getNextWds(input);
    if (client.hasFlag(LOGGED) == false)
    {
        throw (std::runtime_error("Client not logged in"));
    }
    if (item[0] == '#')
    {
        //item is channel
        Channel channel = server.getChannel(item);
        if (channel.isOperator(client))
        {
            try
            {
                channel.setMode(mode_char);
                client.send("[debug] implemented so badly\r\n");
            }
            catch (std::runtime_error& err)
            {
                client.send("MODE :");
                client.send(err.what());
                client.send("\r\n");
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
    std::string token = getHead(input);
    client.send(server.getName() + " PONG :");
    client.send(input);
    client.send("\r\n");
}

void cmdWho(Server& server, Client& client, std::string input)
{
    (void)server;
    if (client.hasFlag(LOGGED) == false)
    {
        throw (std::runtime_error("Client not logged in"));
    }
    std::string arg = getHead(input);
    Client client_who = server.getClient(arg);
    client.send(server.getName());
    client.send(" 352 ");
    client.send(client.getNickname());
    client.send(" * ");
    client_who.send(client.getUsername());
    client.send(" ");
    client_who.send(client.getHostname());
    client.send(" ");
    client.send(server.getName());
    client.send(" ");
    client_who.send(client.getNickname());
    // //client.send(" * TestUser host.example.com irc.example.com TestUser H :0 TestUser\n");
    // //client.send(":ircSchoolProject 315 TestUser TestUser :End of WHO list\n");
}

void cmdUserHost(Server& server, Client& client, std::string input)
{
    (void)server;
    if (client.hasFlag(LOGGED) == false)
    {
        throw (std::runtime_error("Client not logged in"));
    }
    std::string arg = getHead(input);
    if (arg == client.getNickname())
    {
        client.send(":ircSchoolProject 302 TestUser TestUser=@host.example.com+\n");
    }
}

void cmdQuit(Server &server, Client &client, std::string input) {
    (void)server;
    (void)input;
    if (client.hasFlag(LOGGED) == false)
    {
        throw (std::runtime_error("Client not logged in"));
    }
    client.send("quiting");
    client.clientCleanup();
}

void cmdPrivmsg(Server& server, Client& client, std::string input)
{
    (void)server;
    if (client.hasFlag(LOGGED) == false)
    {
        throw (std::runtime_error("Client not logged in"));
    }
    std::string name = getHead(input);
    name = trim(name, OPERATOR_OP);
    std::string msg = getNextWds(input);
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
