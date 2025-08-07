/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capabilities.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:43:27 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/07 12:40:51 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by n on 20/07/25.
//

#ifndef CAPABILITIES_HPP
# define CAPABILITIES_HPP


# include "handle.hpp"

# define PASSWD_OK "passwd_ok"

class Client;

extern const struct s_cmd cap_tab[];

void multiPrefix(Server &server, Client &client, std::string input);
void capLs(Server &server, Client &client, std::string args);
void capReq(Server &server, Client &client, std::string caps);
void capEnd(Server &server, Client &client, std::string caps);
bool isCap(const std::string& cap);
void server_banner(Client &client, Server &server);

#endif //CAPABILITIES_HPP
