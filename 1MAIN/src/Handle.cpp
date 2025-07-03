//
// Created by nidionis on 02/07/25.
//

#include <Handle.hpp>

std::string getHead(const std::string& str) {
    size_t pos = str.find(' ');
    return (pos != std::string::npos) ? str.substr(0, pos) : str;
}

std::string getNextWd(const std::string& str) {
    size_t pos = str.find(' ');
    if (pos == std::string::npos) {
        return "";
    }
    while (pos < str.length() && std::isspace(str[pos])) {
        pos++;
    }
    return str.substr(pos);
}

void cmdCap(Server &server, Client &client, std::string input) {
    (void) server;
    (void) client;
    (void) input;
}

void cmdNick(Server &server, Client &client, std::string input) {
    (void) server;
    server.sendClient(client, "NICK :You are now known as " + client.getNickname() + "\r\n");
    server.sendClient(client, "input: " + input + "\r\n");
    // Handle NICK command
}

void cmdUser(Server &server, Client &client, std::string input) {
    (void) server;
    server.sendClient(client, "USER :You are now known as " + client.getNickname() + "\r\n");
    server.sendClient(client, "input: " + input + "\r\n");
    // Handle USER command
}

void cmdJoin(Server &server, Client &client, std::string input) {
    (void) server;
    server.sendClient(client, "JOIN :You are now known as " + client.getNickname() + "\r\n");
    server.sendClient(client, "input: " + input + "\r\n");
    // Handle JOIN command
}

void cmdPart(Server &server, Client &client, std::string input) {
    (void) server;
    server.sendClient(client, "PART :You are now known as " + client.getNickname() + "\r\n");
    server.sendClient(client, "input: " + input + "\r\n");
    // Handle PART command
}

void cmdPrivmsg(Server &server, Client &client, std::string input) {
    (void) server;
    server.sendClient(client, "PRIVMSG :You are now known as " + client.getNickname() + "\r\n");
    server.sendClient(client, "input: " + input + "\r\n");
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
    std::string cmd_arg = getNextWd(input);
    for (int i = 0; commands[i].f != NULL; i++) {
        if ((cmd_flg == commands[i].header)) {
            commands[i].f(server, client, input);
            return;
        }
    }
}
