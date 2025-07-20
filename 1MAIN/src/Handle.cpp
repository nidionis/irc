//
// Created by nidionis on 02/07/25.
//

#include <Handle.hpp>
#include <Channel.hpp>
#include <iostream>

void cmdCap(Server &server, Client &client, std::string args) {
    (void) server;
    std::cout << "[cmdCap] args:" << args << std::endl;
    if (getHead(args) == "LS") {
        capLS(server, client, getNextWds(args));
    } else if (getHead(args) == "REQ") {
        capREQ(server, client, getNextWds(args));
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
    client.newChannel(channel_str);
}

void cmdPart(Server &server, Client &client, std::string input) {
    (void) server;
    client.send("input: " + input + "\r\n");
    // Handle PART command
}

void cmdPrivmsg(Server &server, Client &client, std::string input) {
    (void) server;
    client.send("input: " + input + "\r\n");
    // Handle PRIVMSG command
}

static const struct s_cmd commands [] = {
        {"CAP",     &cmdCap },
        {"NICK",    &cmdNick},
        {"USER",    &cmdUser},
        {"JOIN",    &cmdJoin},
        {"PART",    &cmdPart},
        {"PRIVMSG", &cmdPrivmsg},
        {"",        NULL} // Terminator
};

void processCommand(Server &server, Client &client, std::string input) {
    std::string cmd_flg = getHead(input);
    std::string cmd_arg = getNextWds(input);
    for (int i = 0; commands[i].f != NULL; i++) {
        if ((cmd_flg == commands[i].header)) {
            commands[i].f(server, client, cmd_arg);
            return;
        }
    }
    client.send("[processCommand]: Invalid command\n");
}
