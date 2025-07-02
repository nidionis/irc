//
// Created by nidionis on 02/07/25.
//

#include <Handle.hpp>

void cmdNick(Server &server, Client &client, std::string input) {
    server.sendClient(client, "NICK :You are now known as " + client.getNickname() + "\r\n");
    server.sendClient(client, "input: " + input + "\r\n");
    // Handle NICK command
}

void cmdUser(Server &server, Client &client, std::string input) {
    server.sendClient(client, "USER :You are now known as " + client.getNickname() + "\r\n");
    server.sendClient(client, "input: " + input + "\r\n");
    // Handle USER command
}

void cmdJoin(Server &server, Client &client, std::string input) {
    server.sendClient(client, "JOIN :You are now known as " + client.getNickname() + "\r\n");
    server.sendClient(client, "input: " + input + "\r\n");
    // Handle JOIN command
}

void cmdPart(Server &server, Client &client, std::string input) {
    server.sendClient(client, "PART :You are now known as " + client.getNickname() + "\r\n");
    server.sendClient(client, "input: " + input + "\r\n");
    // Handle PART command
}

void cmdPrivmsg(Server &server, Client &client, std::string input) {
    server.sendClient(client, "PRIVMSG :You are now known as " + client.getNickname() + "\r\n");
    server.sendClient(client, "input: " + input + "\r\n");
    // Handle PRIVMSG command
}

static const struct s_cmd commands[] = {
        {"NICK",    &cmdNick},
        {"USER",    &cmdUser},
        {"JOIN",    &cmdJoin},
        {"PART",    &cmdPart},
        {"PRIVMSG", &cmdPrivmsg},
        {"",        NULL} // Terminator
};

void processCommand(Server &server, Client &client, std::string input) {
    std::istringstream iss(input);
    std::string command;
    iss >> command;

    for (int i = 0; commands[i].f != NULL; i++) {
        if ((command == commands[i].header)) {
            commands[i].f(server, client, input);
            return;
        }
    }
}
