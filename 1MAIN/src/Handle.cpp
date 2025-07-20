//
// Created by nidionis on 02/07/25.
//

#include <Handle.hpp>
#include <Channel.hpp>
#include <iostream>

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
    std::string head = (pos != std::string::npos) ? str.substr(0, pos) : str;
    return trim(head);
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

std::string isValidName(const std::string& str) {
    if (str.length() > 0 && str.length() < LEN_MAX_NAME) {
        return str;
    }
    throw (std::runtime_error("Invalid name"));
}

void multiPrefix(Server &server, Client &client, std::string input) {
    (void) server;
    (void) client;
    client.send("from function multiPrefix\r\n");
    client.send("returns prefixes set in macro USER_PREFIXES (Client.hpp)");
    client.send("\r\n");
    for (int i = 0; USER_PREFIXES[i]; i++) {
        client.send(" ");
        client.send(std::string(1, USER_PREFIXES[i]));
    }
    client.send("args " + input + "not managed\r\n");
}

static const struct s_cmd req[] = {
        {"multi-prefix",    &multiPrefix },
        {"",                NULL }
};

static void capLS(Server &server, Client &client, std::string args) {
    (void) server;
    (void) args;
    client.send(" CAP * LS : ");
    for (int i = 0; req[i].f; i++) {
        client.send(" ");
        client.send(req[i].header);
    }
    client.send("\nCAP END");
    client.send("\n");
}

static void capREQ(Server &server, Client &client, std::string caps) {
    (void) server;
    std::string cap = getHead(caps);
    std::cout << "[capREQ] caps:" << caps << std::endl;
    while (!cap.empty()) {
        for (int i = 0; req[i].f; i++) {
            std::cout << "req[i].header: " << req[i].header << std::endl;
            std::cout << "cap: " << cap << std::endl;
            if (cap == req[i].header) {
                req[i].f(server, client, "");
                return;
            }
            caps = getNextWds(caps);
            cap = getHead(caps);
        }
    }
    client.send("caps");
    client.send(": Invalid CAP command");
}

void cmdCap(Server &server, Client &client, std::string args) {
    (void) server;
    std::cout << "[cmdCap] args:" << args << std::endl;
    if (getHead(args) == "LS") {
        capLS(server, client, getNextWds(args));
    } else if (getHead(args) == "REQ") {
        capREQ(server, client, getNextWds(args));
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
    }
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
