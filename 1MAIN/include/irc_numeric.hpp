/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_numeric.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irc-bot                                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:00:00 by irc-bot           #+#    #+#             */
/*   Updated: 2025/09/07 00:00:00 by irc-bot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <sstream>

#include "Server.hpp"
#include "Client.hpp"

// Replies (partial set)
#define RPL_WELCOME           001
#define RPL_YOURHOST          002
#define RPL_CREATED           003
#define RPL_MYINFO            004
#define RPL_ISUPPORT          005
#define RPL_USERHOST          302
#define RPL_ENDOFWHO          315
#define RPL_WHOREPLY          352

// Errors (partial set)
#define ERR_NOSUCHNICK        401
#define ERR_NOSUCHSERVER      402
#define ERR_NOSUCHCHANNEL     403
#define ERR_CANNOTSENDTOCHAN  404
#define ERR_UNKNOWNCOMMAND    421
#define ERR_NONICKNAMEGIVEN   431
#define ERR_ERRONEUSNICKNAME  432
#define ERR_NICKNAMEINUSE     433
#define ERR_NOTREGISTERED     451
#define ERR_NEEDMOREPARAMS    461
#define ERR_ALREADYREGISTRED  462
#define ERR_PASSWDMISMATCH    464
#define ERR_CHANNELISFULL     471
#define ERR_UNKNOWNMODE       472
#define ERR_INVITEONLYCHAN    473
#define ERR_BANNEDFROMCHAN    474
#define ERR_BADCHANNELKEY     475
#define ERR_CHANOPRIVSNEEDED  482

inline std::string ircJoinSpaces(const std::vector<std::string>& items) {
	std::ostringstream oss;
	for (size_t i = 0; i < items.size(); ++i) {
		if (i) oss << ' ';
		oss << items[i];
	}
	return oss.str();
}

// Build a numeric:
// :<server> <3-digit> <target> [middle...] [:trailing]\r\n
inline std::string makeNumeric(Server& server,
                               const std::string& target,
                               int code,
                               const std::vector<std::string>& middle = {},
                               const std::string& trailing = "") {
	std::ostringstream oss;
	oss << ":" << server.getName() << ' ';
	oss.width(3);
	oss.fill('0');
	oss << code;
	if (!target.empty()) {
		oss << ' ' << target;
	}
	if (!middle.empty()) {
		oss << ' ' << ircJoinSpaces(middle);
	}
	if (!trailing.empty()) {
		oss << " :" << trailing;
	}
	oss << "\r\n";
	return oss.str();
}

inline std::string makeNumeric(Server& server,
                               Client& client,
                               int code,
                               const std::vector<std::string>& middle = {},
                               const std::string& trailing = "") {
	const std::string tgt = client.getNickname().empty() ? std::string("*") : client.getNickname();
	return makeNumeric(server, tgt, code, middle, trailing);
}

inline void sendNumeric(Server& server,
                        Client& client,
                        int code,
                        const std::vector<std::string>& middle = {},
                        const std::string& trailing = "") {
	client.send(makeNumeric(server, client, code, middle, trailing));
}
