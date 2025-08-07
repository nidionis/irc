/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:43:34 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/07 12:46:08 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by nidionis on 02/07/25.
//

#ifndef HANDLE_HPP
# define HANDLE_HPP

# include <iostream>

# include "Client.hpp"
# include "capabilities.hpp"

# define PASSWD_OK "passwd_ok"

# define AVAILABLE_MODE "itkol"
//# define INVITE_ONLY    0
//# define TOPIC_RESTRICT 1
//# define KEY            2
# define OP             3
//# define LIMIT          4
//# define TOPIC_MAX_LEN  2048

# define OPERATOR_OP "%@"
//# define HALFOP 0
//# define CHANOP 1

class Server;
class Client;

// Function declarations
void processCommand(Server &server, Client &client, std::string input);

// Command function declarations
void cmdCap(Server &server, Client &client, std::string args);
void cmdNick(Server &server, Client &client, std::string input);
void cmdUser(Server &server, Client &client, std::string input);
void cmdJoin(Server &server, Client &client, std::string input);
void cmdTopic(Server &server, Client &client, std::string input);
void cmdPrivmsg(Server &server, Client &client, std::string input);
void cmdMode(Server &server, Client &client, std::string input);
void cmdKick(Server &server, Client &client, std::string input);
void cmdPing(Server &server, Client &client, std::string input);
void cmdWho(Server &server, Client &client, std::string input);
void cmdUserHost(Server &server, Client &client, std::string input);
void cmdPass(Server &server, Client &client, std::string input);
void cmdInvite(Server &server, Client &client, std::string input);
void cmdQuit(Server &server, Client &client, std::string input);
void cmdDebug(Server& server, Client& client, std::string args);

typedef void (*CommandFunc)(Server &server, Client &client, std::string input);

struct s_cmd {
    const char* header;
    CommandFunc f;
};

static const struct s_cmd commands [] = {
    {"CAP",      &cmdCap },
    {"NICK",     &cmdNick},
    {"USER",     &cmdUser},
    {"JOIN",     &cmdJoin},   // channel : test later
    {"KICK",     &cmdKick},   // channel : test later
    {"TOPIC",    &cmdTopic},  // channel : test later
    {"MODE",     &cmdMode},   // channel : test later
    {"PING",     &cmdPing},
    {"WHO",      &cmdWho},
    {"USERHOST", &cmdUserHost},
    {"MSG",      &cmdPrivmsg}, // ! MSG does not exist, use PRIVMSG !
    {"PRIVMSG",  &cmdPrivmsg},
    {"PASS",     &cmdPass}, // special : test later
    {"INVITE",   &cmdInvite},
    {"QUIT",     &cmdQuit},
    {"DEBUG",     &cmdDebug},
    {"",        NULL} // Terminator
};

#endif // HANDLE_HPP
