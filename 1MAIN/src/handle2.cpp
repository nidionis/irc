/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:40:28 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/07 14:18:38 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "handle.hpp"

/*
Command: JOIN
Parameters: <channel>{,<channel>} [<key>{,<key>}]
Alt Params: 0

[13:49] << JOIN #NewChannel%0A
[13:49] >> :NickName!~UserName@45.148.156.203 JOIN #NewChannel * :RealName%0A
[13:49] << MODE #NewChannel%0A
[13:49] >> :swepipe.esper.net MODE #NewChannel +nt%0A
[13:49] >> :swepipe.esper.net 353 NickName = #NewChannel :@NickName!~UserName@45.148.156.203%0A
[13:49] >> :swepipe.esper.net 366 NickName #NewChannel :End of /NAMES list.%0A
[13:49] >> :swepipe.esper.net 324 NickName #NewChannel +nt%0A
[13:49] >> :swepipe.esper.net 329 NickName #NewChannel 1754567346%0A
[13:49] << MODE #NewChannel +b%0A
[13:49] >> :swepipe.esper.net 368 NickName #NewChannel :End of Channel Ban List%0A

// joining several channels at once :
[14:12] << JOIN #test123,#test456,#test789%0A
[14:12] >> :NickName!~UserName@45.148.156.203 JOIN #test123 * :RealName%0A
[14:12] << MODE #test123%0A
[14:12] >> :swepipe.esper.net MODE #test123 +nt%0A
[14:12] >> :swepipe.esper.net 353 NickName = #test123 :@NickName!~UserName@45.148.156.203%0A
[14:12] >> :swepipe.esper.net 366 NickName #test123 :End of /NAMES list.%0A
[14:12] >> :NickName!~UserName@45.148.156.203 JOIN #test456 * :RealName%0A
[14:12] >> :swepipe.esper.net MODE #test456 +nt%0A
[14:12] >> :swepipe.esper.net 353 NickName = #test456 :@NickName!~UserName@45.148.156.203%0A
[14:12] >> :swepipe.esper.net 366 NickName #test456 :End of /NAMES list.%0A
[14:12] >> :NickName!~UserName@45.148.156.203 JOIN #test789 * :RealName%0A
[14:12] >> :swepipe.esper.net MODE #test789 +nt%0A
[14:12] >> :swepipe.esper.net 353 NickName = #test789 :@NickName!~UserName@45.148.156.203%0A
[14:12] >> :swepipe.esper.net 366 NickName #test789 :End of /NAMES list.%0A
[14:12] >> :swepipe.esper.net 324 NickName #test123 +nt%0A
[14:12] >> :swepipe.esper.net 329 NickName #test123 1754568766%0A
[14:12] << MODE #test123 +b%0A
[14:12] >> :swepipe.esper.net 368 NickName #test123 :End of Channel Ban List%0A
[14:12] << MODE #test456%0A
[14:12] >> :swepipe.esper.net 324 NickName #test456 +nt%0A
[14:12] >> :swepipe.esper.net 329 NickName #test456 1754568766%0A
[14:12] << MODE #test456 +b%0A
[14:12] >> :swepipe.esper.net 368 NickName #test456 :End of Channel Ban List%0A
[14:13] << MODE #test789%0A
[14:13] >> :swepipe.esper.net 324 NickName #test789 +nt%0A
[14:13] >> :swepipe.esper.net 329 NickName #test789 1754568766%0A
[14:13] << MODE #test789 +b%0A
[14:13] >> :swepipe.esper.net 368 NickName #test789 :End of Channel Ban List%0A
*/

void cmdJoin(Server& server, Client& client, std::string input)
{
    std::string channel_str = popWd(input);
    std::string key = lastWord(input);
    Channel channel;

    if (client.isLogged() == false)
    {
        client.must_kill = true;
        return ;
    }
    if (channel_str[0] == '#' && isValidName(channel_str.substr(1)))
    {
        try
        {
            channel = server.getChannel(channel_str);
            try
            {
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

/*
Command: PART
Parameters: <channel>{,<channel>} [<reason>]

// SUCCESS MESSAGE with no reason
[13:57] << PART #TestChannel :%0A
[13:57] >> :NickName!~UserName@45.148.156.203 PART #TestChannel%0A

// SUCCESS MESSAGE with reason
[14:05] << PART #test654 :hello%0A
[14:05] >> :NickName!~UserName@45.148.156.203 PART #test654 :hello%0A

// SUCCESS messages, part 3 channels at once
[14:14] << PART #test123,#test456,#test789 :%0A
[14:14] >> :NickName!~UserName@45.148.156.203 PART #test123%0A
[14:14] >> :NickName!~UserName@45.148.156.203 PART #test456%0A
[14:14] >> :NickName!~UserName@45.148.156.203 PART #test789%0A

// error : channel exist but user is not part of it
[14:00] << PART #test :%0A
[14:00] >> :swepipe.esper.net 442 NickName #test :You're not on that channel%0A

// error : channel does not exist
[14:00] << PART #test54654 :%0A
[14:00] >> :swepipe.esper.net 403 NickName #test54654 :No such channel%0A
*/

void cmdPart(Server& server, Client& client, std::string args)
{
    return ;
}

void cmdMode(Server& server, Client& client, std::string input)
{
    std::string item = popWd(input);
    std::string mode_chars = popWd(input);
    std::string args = input;
    Channel channel;

    if (client.isLogged() == false)
    {
        client.must_kill = true;
        return ;
    }
    if (item[0] == '#') {
        try {
            channel = server.getChannel(item);
        }
        catch (const std::runtime_error& err)
        {
            throw std::runtime_error(err.what());
        }
        if (channel.isOperator(client))
        {
            if (mode_chars[0] == '+' || mode_chars[0] == '-')
            {
                if (!strchr(AVAILABLE_MODE, mode_chars[1]))
                {
                    throw std::runtime_error("MODE :Invalid mode\r\n");
                }
            }
            if (mode_chars[0] == '+')
            {
                if (mode_chars[1] == 'k')
                {
                    channel.setKey(args);
                }
                else
                {
                    channel.setMode(mode_chars[1]);
                }
            }
            else if (mode_chars[0] == '-')
            {
                if (mode_chars[1] == 'k')
                {
                    channel.setKey("");
                }
                else
                {
                    channel.delMode(mode_chars[1]);
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
    std::string channel_str = popWd(input);
    Client& kicked = server.getClient(getHead(input));
    std::string reason = getNextWds(input);
    if (client.isLogged() == false)
    {
        client.must_kill = true;
        return ;
    }
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
    if (client.isLogged() == false)
    {
        client.must_kill = true;
        return ;
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

void cmdPrivmsg(Server& server, Client& client, std::string input)
{
    std::string name = getHead(input);
    name = trim(name, OPERATOR_OP);
    std::string msg = getNextWds(input);
    if (client.isLogged() == false)
    {
        client.must_kill = true;
        return ;
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

void cmdInvite(Server &server, Client &client, std::string input) {
    Client dest;
    Channel channel;
    if (client.isLogged() == false)
    {
        client.must_kill = true;
        return ;
    }
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

    if (channel.hasMode('i')) {
        if (channel.isOperator(client)) {
            throw std::runtime_error(client.getNickname() + channel.getName() + "you are not operrator");
        }
    }
    dest.send(client.getNickname() + " INVITE " + dest.getNickname() + " " + channel.getName() + "\r\n");
    channel.setClient(dest);
}
