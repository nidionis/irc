//
// Created by nidionis on 02/07/25.
//

#ifndef HANDLE_HPP
#define HANDLE_HPP

#include <string>
#include <main.hpp>

// Define the function pointer type
typedef void (*CommandFunc)(Server &server, Client &client, std::string input);

// Define the struct with the full definition
struct s_cmd {
    const char* header;
    CommandFunc f;
};

// Function declarations
void processCommand(Server &server, Client &client, std::string input);
bool cmpHead(const std::string& input, const char* header);

// Command function declarations
void cmdNick(Server &server, Client &client, std::string input);
void cmdUser(Server &server, Client &client, std::string input);
void cmdJoin(Server &server, Client &client, std::string input);
void cmdPart(Server &server, Client &client, std::string input);
void cmdPrivmsg(Server &server, Client &client, std::string input);

#endif // HANDLE_HPP
