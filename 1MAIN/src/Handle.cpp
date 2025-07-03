//
// Created by nidionis on 02/07/25.
//

#include <Handle.hpp>

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    if (first == std::string::npos || last == std::string::npos) {
        return "";
    }
    return str.substr(first, last - first + 1);
}

std::string getHead(const std::string& str) {
    size_t pos = str.find(' ');
    return (pos != std::string::npos) ? str.substr(0, pos) : trim(str);
}

std::string getNextWds(const std::string& str) {
    size_t pos = str.find(' ');
    if (pos == std::string::npos) {
        return "";
    }
    while (pos < str.length() && std::isspace(str[pos])) {
        pos++;
    }
    return str.substr(pos);
}

void cmdCap(Server &server, Client &client, std::string args) {
    (void) server;
    //should wait NICK an USER
    if (getHead(args) == "LS") {
        server.sendClient(client, "CAP * LS :");
        server.sendClient(client, "une liste de commandes implementees");
        server.sendClient(client, "\n");;
    }
}

void cmdNick(Server &server, Client &client, std::string input) {
    (void) server;
    client.setNickname(input);
    server.sendClient(client, "NICK :You are now known as " + client.getNickname() + "\r\n");
    server.sendClient(client, "input: " + input + "\r\n");
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
    std::string cmd_arg = getNextWds(input);
    for (int i = 0; commands[i].f != NULL; i++) {
        if ((cmd_flg == commands[i].header)) {
            commands[i].f(server, client, cmd_arg);
            return;
        }
    }
}
