//
// Created by nidionis on 02/07/25.
//

#include <Handle.hpp>
#include <Channel.hpp>
#include <iostream>

#include "../include/utils_strings.hpp"

void cmdCap(Server &server, Client &client, std::string args) {
    (void) server;
    std::cout << "[cmdCap] args:" << args << std::endl;
    if (getHead(args) == "LS") {
        capLs(server, client, getNextWds(args));
    } else if (getHead(args) == "REQ") {
        capReq(server, client, getNextWds(args));
    } else if (getHead(args) == "END") {
        capEnd(server, client, getNextWds(args));
    }
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
    if (client.hasFlag(LOGGED) == false) {
        throw (std::runtime_error("Client not logged in"));
    }
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

void cmdMode(Server &server, Client &client, std::string input) {
    (void) server;
    (void) client;
    (void) input;

}

void processCommand(Server &server, Client &client, std::string input) {
    std::string cmd_flg = upperCase(getHead(input));
    std::string cmd_arg = getNextWds(input);
    for (int i = 0; commands[i].f != NULL; i++) {
        if (cmd_flg == commands[i].header) {
            try {
                commands[i].f(server, client, cmd_arg);
            } catch (std::runtime_error &err) {
                client.send(err.what());
                client.send("\n");
            }
            return;
        }
    }
    client.send("[processCommand]: Invalid command\n");
}
