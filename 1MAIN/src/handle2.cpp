/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidionis <nidionis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:40:28 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/31 16:39:11 by nidionis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "handle.hpp"

/*
Command: JOIN
Parameters: <channel>{,<channel>} [<key>{,<key>}]
Alt Params: 0

<< JOIN #test654%0A
>> JOIN :Channel does not exist%0A
>> JOIN : Succefully created%0A
>> :NickName!~UserName@10.12.4.1 JOIN #test654 * :RealName%0A
<< MODE #test654%0A
<< MODE #test654 +b%0A

<< JOIN #test654%0A
>> :NickName!~UserName@45.148.156.203 JOIN #test654 * :RealName%0A
<< MODE #test654%0A
>> :swepipe.esper.net MODE #test654 +nt%0A
>> :swepipe.esper.net 353 NickName = #test654 :@NickName!~UserName@45.148.156.203%0A
>> :swepipe.esper.net 366 NickName #test654 :End of /NAMES list.%0A
>> :swepipe.esper.net 324 NickName #test654 +nt%0A
>> :swepipe.esper.net 329 NickName #test654 1756638341%0A
<< MODE #test654 +b%0A
>> :swepipe.esper.net 368 NickName #test654 :End of Channel Ban List%0A

=> do not implement the MODE replies in cmdJoin(), those will be handled in cmdMode()
*/

void	cmdJoin(Server& server, Client& client, std::string args)
{
    std::string channel_str = popWd(args);
    std::string key = popWd(args);
    Channel channel;
	std::string	reply_success = ":" + client.getNickname() + "!~" + client.getUsername()
		+ "@" + getLocalIPv4Address() + " JOIN " + channel_str + " * :" + client.getRealname() + "\r\n";

    if (client.isLogged() == false)
    {
        client.setmust_kill(true);
        return ;
    }
    if (channel_str[0] == '#' && channel_str.length() < LEN_MAX_NAME)// && isValidName(channel_str.substr(1)))
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
                client.send(reply_success);
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
                client.send(reply_success);
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

// error : channel exist but user is not part of it
[14:00] << PART #test :%0A
[14:00] >> :swepipe.esper.net 442 NickName #test :You're not on that channel%0A

// error : channel does not exist
[14:00] << PART #test54654 :%0A
[14:00] >> :swepipe.esper.net 403 NickName #test54654 :No such channel%0A
*/

void	cmdPart(Server& server, Client& client, std::string args)
{
    (void)server;
    (void)client;
    (void)args;
	//client.send("coucou");
    Channel channel;
	std::string str_channel = popWd(args);
	if (str_channel == "") {
		server.sendHead(client, "461");
		client.send(client.getNickname());
		client.send(" PART :");
		client.send("not enough parameters\r\n");
	}
	//client.send("str_channel:");
	//client.send(str_channel);
	try {
		channel = server.getChannel(str_channel);
		std::string msg = client.getNickname() + " is leaving " + channel.getName();
		channel.spawn(msg);
		server.delChannel(channel);
		client.delChannel(channel);
	} catch (std::runtime_error &err) {
		client.send(client.getNickname());
		client.send(" ");
		client.send(str_channel);
		client.send(" :No such channel");
	}
    return ;
}

/*
Command: MODE
Parameters: <target> [<modestring> [<mode arguments>...]]
  
/!\ not tested yet /!\
*/

void	cmdMode(Server& server, Client& client, std::string args)
{
	(void)server;
	(void)client;
	(void)args;
	std::string	item = popWd(args);
	std::string	mode_chars = popWd(args);
	Channel		channel;

	if (client.isLogged() == false)
		{ client.setmust_kill(true); return ; }
	if (item[0] == '#') {
		try { channel = server.getChannel(item); }
		catch (const std::runtime_error& err)
			{ throw std::runtime_error(err.what()); }
		if (channel.isOperator(client))
		{
			if (mode_chars[0] == '+' || mode_chars[0] == '-')
			{
				if (!strchr(AVAILABLE_MODE, mode_chars[1]))
					{ throw std::runtime_error("MODE :Invalid mode\r\n"); }
			}
			if (mode_chars[0] == '+')
			{
				if (mode_chars[1] == 'k')
					{ channel.setKey(args); }
				else
					{ channel.setMode(mode_chars[1]); }
			}
			else if (mode_chars[0] == '-')
			{
				if (mode_chars[1] == 'k')
					{ channel.setKey(""); }
				else
					{ channel.delMode(mode_chars[1]); }
			}
		}
	}
	else
	{
		if (server.clientHasUser(item))
			{ std::cout << "[debug] do something with user here" << std::endl; }
	}
	return ;
}

/*
Command: KICK
Parameters: <channel> <user> *( "," <user> ) [<comment>]

// kick self (no comment)
[15:09] << KICK #test321 NickName :%0A
[15:09] >> :NickName!~UserName@45.148.156.203 KICK #test321 NickName :NickName%0A

// kick other (no comment)
[15:14] << KICK #test456 lahlsweh :%0A
[15:14] >> :NickName!~UserName@45.148.156.203 KICK #test456 lahlsweh :lahlsweh%0A

// kick self (comment)
[15:11] << KICK #test456 NickName :somecomment%0A
[15:11] >> :NickName!~UserName@45.148.156.203 KICK #test456 NickName :somecomment%0A

// kick other (comment)
[15:15] << KICK #test456 lahlsweh :somecomment%0A
[15:15] >> :NickName!~UserName@45.148.156.203 KICK #test456 lahlsweh :somecomment%0A

// not operator (no comment)
[15:10] << KICK #test123 lahlsweh :%0A
[15:10] >> :calamity.esper.net 482 NickName #test123 :You're not a channel operator%0A

// not operator (comment)
[15:12] << KICK #test123 lahlsweh :somecomment%0A
[15:12] >> :calamity.esper.net 482 NickName #test123 :You're not a channel operator%0A

// bad format
[15:13] << KICK #test456 NickName:somecomment%0A
[15:13] >> :calamity.esper.net 401 NickName NickName:somecomment :No such nick/channel%0A

// bad format
[15:13] << KICK #test456 NickName: somecomment%0A
[15:13] >> :calamity.esper.net 401 NickName NickName: :No such nick/channel%0A

// wrong channel
[15:19] << KICK #test456 lahlsweh :somereason%0A
[15:19] >> :calamity.esper.net 403 NickName #test456 :No such channel%0A

// user not in channel
[15:19] << KICK #test951 lahlsweh :somereason%0A
[15:19] >> :calamity.esper.net 441 NickName lahlsweh #test951 :They aren't on that channel%0A
*/

void	cmdKick(Server& server, Client& client, std::string args)
{
	std::string	channel_str = popWd(args);
	Client		&kicked = server.getClient(getHead(args));
	std::string	reason = getNextWds(args);

	if (client.isLogged() == false) { client.setmust_kill(true); return ; }
	if (channel_str[0] == '#' && isValidName(channel_str.substr(1)))
	{
		try
		{
			Channel&	channel = server.getChannel(channel_str);
			if (channel.isOperator(client))
			{
				try { channel.delClient(client); }
				catch (std::runtime_error& err)
				{
					client.send("JOIN : Nick ");
					client.send(kicked.getNickname());
					client.send("not in the channel\r\n");
				}
			}
			else { client.send("JOIN :You are not an operator of the channel\r\n"); }
		}
		catch (const std::runtime_error& err)
			{ client.send("JOIN :Channel does not exist\r\n"); }
	}
	return ;
}

/*
Command: TOPIC
Parameters: <channel> [<topic>]
/!\ not tested yet /!\
*/

void	cmdTopic(Server& server, Client& client, std::string args)
{
	(void)server;
	std::string	channel_str = getHead(args);
	std::string	topic = getNextWds(args);
	std::string	err_str;

	if (client.isLogged() == false) { client.setmust_kill(true); return ; }
	if (channel_str[0] == '#' && isValidName(channel_str.substr(1)))
	{
		try
		{
			Channel&	channel = server.getChannel(channel_str);
			if (channel.isOperator(client))
				{ channel.setTopic(topic); }
		}
		catch (const std::runtime_error& err)
		{
			client.send("TOPIC : " + err_str + "\r\n");
		}
	}
	return ;
}

/*
Command: PRIVMSG
Parameters: <target>{,<target>} <text to be sent>

// when DMing an other user, server will not resond whethe it succeeded or failed
[15:50] << PRIVMSG test2 :ggg%0A

// sending to channel, success but no server reply
[15:52] << PRIVMSG #test123 :testchannel%0A

// message received, server must process it but not respond
[15:51] >> :test2!~lahlsweh@45.148.156.203 PRIVMSG NickName :trert%0A

// channel does not exist
[15:54] << PRIVMSG #badchan :test%0A
[15:54] >> :calamity.esper.net 401 NickName #badchan :No such nick/channel%0A

// user does not exist
[15:55] << PRIVMSG baduser789 :test%0A
[15:55] >> :calamity.esper.net 401 NickName baduser789 :No such nick/channel%0A

// no message
[16:45] << PRIVMSG NickName%0A
[16:45] >> :anarchy.esper.net 412 NickName :No text to send%0A

// bad format
[16:46] << PRIVMSG :NickName :test%0A
[16:46] >> :anarchy.esper.net 412 NickName :No text to send%0A
*/
void	cmdPrivmsg(Server& server, Client& client, std::string args)
{
	std::string	name = getHead(args);
	std::string	msg = getNextWds(args);
	Client		dest;

	name = trim(name, OPERATOR_OP);
	if (client.isLogged() == false) { client.setmust_kill(true); return ; }
	if (name[0] == '#' && isValidName(name.substr(1)))
	{
		try
		{
			Channel&	channel = server.getChannel(name);
			channel.spawn(client.getNickname() + " : " + msg);
		}
		catch (const std::runtime_error& err)
			{ client.send(err.what()); }
	}
	else if (server.clientHasNick(name))
	{
		dest = server.getClient(name);
		dest.send(client.getNickname() + " : " + msg + "\r\n");
	}
	else { throw std::runtime_error("PRIVMSG : channel or client not found"); }
	throw (std::runtime_error("PRIVMSG : Failure"));
}

/*
Command: INVITE
Parameters: <nickname> <channel>

// success
[15:56] << INVITE test1 #test123%0A
[15:56] >> :calamity.esper.net 341 NickName test1 #test123%0A

// invite received
[15:57] >> :test2!~lahlsweh@45.148.156.203 INVITE NickName :#test456%0A

// user exist, channel does not exist
[15:58] << INVITE test2 #test987%0A
[15:58] >> :calamity.esper.net 403 NickName #test987 :No such channel%0A

// user already on channel
[15:58] << INVITE test2 #test456%0A
[15:58] >> :calamity.esper.net 443 NickName test2 #test456 :is already on channel%0A

// user does not exist, channel exist
[15:58] << INVITE test46542 #test456%0A
[15:58] >> :calamity.esper.net 401 NickName test46542 :No such nick/channel%0A

// user not on channel
[16:00] << INVITE test2 #test789%0A
[16:00] >> :calamity.esper.net 442 NickName #test789 :You're not on that channel%0A

// not operator
[16:03] << INVITE test1 #test456%0A
[16:03] >> :calamity.esper.net 482 NickName #test456 :You're not a channel operator%0A
*/

void	cmdInvite(Server& server, Client& client, std::string args)
{
	Client	dest;
	Channel	channel;

	if (client.isLogged() == false) { client.setmust_kill(true); return ; }
	try { dest = server.getClient(popWd(args)); }
	catch (std::runtime_error& err)
		{ throw std::runtime_error(err.what()); }
	try { channel = server.getChannel(popWd(args)); }
	catch (std::runtime_error& err)
		{ throw std::runtime_error(err.what()); }
	if
	(channel.hasMode('i'))
	{
		if (channel.isOperator(client))
			{ throw (std::runtime_error(client.getNickname() + channel.getName() + "you are not operrator")); }
	}
	dest.send(client.getNickname() + " INVITE " + dest.getNickname() + " " + channel.getName() + "\r\n");
	channel.setClient(dest);
	return ;
}
