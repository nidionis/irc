/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capabilities.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:43:45 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/07 12:28:09 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by n on 20/07/25.
//

#include "capabilities.hpp"

const struct s_cmd cap_tab[] = {
    {"multi-prefix",    &multiPrefix },
    {"",                NULL }
};

bool isCap(const std::string& cap)
{
    if (cap.empty()) {
        return false;
    }

    for (size_t i = 0; cap_tab[i].header[0] != '\0'; ++i) {
        if (cap == cap_tab[i].header) {
            return true;
        }
    }
    return false;
}

void multiPrefix(Server& server, Client& client, std::string input)
{
    (void)server;
    (void)client;
    (void)input;
    //if (client.hasCap("multi-prefix")) {
    //    client.resetCap("multi-prefix");
    //} else {
        client.setCap("multi-prefix");
    //}
}

void capLs(Server& server, Client& client, std::string args)
{
    (void)server;
    (void)args;
    client.send(":");
    client.send(server.getName());
    client.send(" CAP * LS :");
    for (int i = 0; cap_tab[i].f; i++)
    {
        client.send(cap_tab[i].header);
        //client.send(" ");
    }
    client.send("\r\n");
}


void capReq(Server& server, Client& client, std::string caps)
{
    (void)server;
    std::cout << "[capReq] caps:" << caps << std::endl;
    std::string cap = popWd(caps);
    //client.send(server.getName());
    std::string messageCAP_ACK;
    if (!cap.empty())
    {
        for (int i = 0; cap_tab[i].f; i++)
        {
            if (cap == cap_tab[i].header)
            {
                cap_tab[i].f(server, client, "");
                if (client.getNickname() != "")
                    messageCAP_ACK = ":" + server.getName() + " CAP " + client.getNickname() + " ACK :" + cap + "\r\n";
                else
                    messageCAP_ACK = ":" + server.getName() + " CAP " + "*" + " ACK :" + cap + "\r\n";
                client.send(messageCAP_ACK);
            } else
            {
                std::string messageCAP_NAK = ":" + server.getName() + " CAP " + client.getNickname() + " NAK :" + cap + "\r\n";
                client.send(messageCAP_NAK);
            }
        }
    }
    // else {
    //    client.send(":" + server.getName() + " 461 " + client.getNickname() + " CAP :Not enough parameters\r\n");
    //}
}

void server_banner(Client &client, Server &server)
{
    if (client.hasFlag("BANNER_SENT"))
        return ;
    std::string message001 = ":Welcome to the ircSchoolProject " + client.getNickname() + '\n';
    std::string message002 = ":Your host is ircSchoolProject[" + server.getIp() + "/" + ft_put_uint16(server.getPort()) + "], running version v1.0\n";
    std::string message003 = ":This server was created Wed Jul 2025 at 12:00:00 EST\n";
    std::string message004 = "ircSchoolProject v1.0 o itkol kl\n";
    std::string message005 = "CHANMODES=o,k,l,it :are supported by this server\n";
    std::string messageMODE = ":" + client.getNickname() + " MODE " + client.getNickname() + " :+i\n";
    server.sendHead(client, "001");
    client.send(message001);
    server.sendHead(client, "002");
    client.send(message002);
    server.sendHead(client, "003");
    client.send(message003);
    server.sendHead(client, "004");
    client.send(message004);
    server.sendHead(client, "005");
    client.send(message005);
    client.send(messageMODE);
    client.setFlag("BANNER_SENT");
}

void capEnd(Server &server, Client &client, std::string caps) {
    (void)server;
    (void)caps;
        client.setFlag("CAP_END");
        if (client.isLogged())
            server_banner(client, server);
}
