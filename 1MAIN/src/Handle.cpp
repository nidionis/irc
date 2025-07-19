//
// Created by nidionis on 02/07/25.
//

#include <Handle.hpp>
#include <Channel.hpp>
#include <iostream>

void cmdCap(Server &server, Client &client, std::string args) {
    (void) server;
    //should wait NICK an USER
    std::cout << "[cmdCap] args: " << args << "\n";
        std::cout << "[cmdCap] getHead: " << getHead(args) << "\n";
    if (getHead(args) == "LS") {
        client.send("CAP * LS :");
        client.send(" CAP * LS : https://ircv3.net/specs/extensions/capability-negotiation");
        client.send("CAP END");
        client.send("\n");;
    }
    // should wait a cap end
}

void cmdNick(Server &server, Client &client, std::string input) {
    (void) server;
    std::string nick = getHead(input);
    if (nick != "") {
        if (client.getNickname() != "") {
            client.send("NICK :Nickname already set\r\n"); return; }
        if (server.hasNick(nick))
            client.send("NICK :Nickname already attributed\r\n");
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
        if (client.getUsername() == "")
            client.setUsername(user);
        else
            client.send("USER :Username already in use\r\n");
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

static const struct s_cmd commands[] = {
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
}
