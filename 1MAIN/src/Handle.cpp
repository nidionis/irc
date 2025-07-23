//
// Created by nidionis on 02/07/25.
//

#include <Handle.hpp>
#include <Channel.hpp>
#include <iostream>

#include "../include/Server.hpp"
#include "../include/utils_strings.hpp"

void cmdCap(Server &server, Client &client, std::string args) {
    (void) server;
    std::cout << "[cmdCap] args:" << args << std::endl;
    if (getHead(args) == "LS") {
        capLs(server, client, getNextWds(args));
    } else if (getHead(args) == "REQ") {
        capReq(server, client, getNextWds(args));
    }
    // should wait a cap end
}

void cmdNick(Server &server, Client &client, std::string input) {
    (void) server;
    std::string nick = getHead(input);
    if (nick != "") {
        if (server.hasNick(nick))
            client.send("NICK :NickName already in use\r\n");
        else
            client.setNickname(nick);
    } else
        client.send("NICK :You are now known as " + client.getNickname() + "\r\n");
}

void cmdUser(Server &server, Client &client, std::string input) {
    (void) server;
    std::string user = getHead(input);
    if (input != "")
    {
        if (server.hasUser(user))
            client.send("USER :Username already in use\r\n");
        else
            client.setUsername(user);
    }
    client.send("USER :You are now known as " + client.getUsername() + "\r\n");
}

void cmdJoin(Server &server, Client &client, std::string input) {
    (void) server;
    (void) client;
    std::string channel_str = getHead(input);
    Channel channel;
    if (channel_str[0] == '#' && isValidName(channel_str.substr(1)))
    {
        try {
            channel = server.getChannel(channel_str);
            try {
                channel.setClient(client);
            } catch (std::runtime_error &err) {
                client.send("JOIN :You are already in the channel\r\n");
            }
        } catch (const std::runtime_error &err) {
            client.send("JOIN :Channel does not exist\r\n");
            try {
                client.newChannel(channel_str);
            } catch (std::runtime_error &err) {
                client.send("JOIN : failed :Too many channels\r\n");
            }
        }
    } else {
        client.send("JOIN :Invalid channel name\r\n");
    }
}

void cmdKick(Server &server, Client &client, std::string input) {
    (void) server;
    (void) client;
    std::string channel_str = getHead(input);
    input = getNextWds(input);
    Client &kicked = server.getClient(getHead(input));
    std::string reason = getNextWds(input);
    if (channel_str[0] == '#' && isValidName(channel_str.substr(1)))
    {
        try {
            Channel &channel = server.getChannel(channel_str);
            if (channel.isAdmin(client)) {
                try {
                    channel.delClient(client);
                } catch (std::runtime_error &err) {
                    client.send("JOIN : Nick");
                    client.send(kicked.getNickname());
                    client.send("not in the channel\r\n");
                }
            } else
                client.send("JOIN :You are not an admin of the channel\r\n");
        } catch (const std::runtime_error &err) {
            client.send("JOIN :Channel does not exist\r\n");
        }
    }
}

void processCommand(Server &server, Client &client, std::string input) {
    std::string cmd_flg = upperCase(getHead(input));
    std::string cmd_arg = getNextWds(input);
    for (int i = 0; commands[i].f != NULL; i++) {
        if (cmd_flg == commands[i].header) {
            commands[i].f(server, client, cmd_arg);
            return;
        }
    }
    client.send("[processCommand]: Invalid command\n");
}