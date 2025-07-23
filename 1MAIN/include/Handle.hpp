//
// Created by nidionis on 02/07/25.
//

#ifndef HANDLE_HPP
#define HANDLE_HPP

#include <string>
#include <main.hpp>

// Function declarations
void processCommand(Server &server, Client &client, std::string input);
bool cmpHead(const std::string& input, const char* header);

// Command function declarations
void cmdCap(Server &server, Client &client, std::string args);
void cmdNick(Server &server, Client &client, std::string input);
void cmdUser(Server &server, Client &client, std::string input);
void cmdJoin(Server &server, Client &client, std::string input);
void cmdPrivmsg(Server &server, Client &client, std::string input);
void cmdMode(Server &server, Client &client, std::string input);
void cmdKick(Server &server, Client &client, std::string input);

// /!\ must be vreated using createChannel only -> use malloc by server and added by client
void createChannel(Server &server, Client &client, std::string channel_str);

typedef void (*CommandFunc)(Server &server, Client &client, std::string input);

struct s_cmd {
    const char* header;
    CommandFunc f;
};

static const struct s_cmd commands [] = {
    {"CAP",     &cmdCap },
    {"NICK",    &cmdNick},
    {"USER",    &cmdUser},
    {"JOIN",    &cmdJoin},
    {"KICK",    &cmdKick},
    {"MODE",    &cmdMode},
    {"",        NULL} // Terminator
};

#endif // HANDLE_HPP
