//
// Created by n on 20/07/25.
//

#include "capabilities.hpp"

#include <string>

#include "../include/Client.hpp"
#include "../include/Handle.hpp"

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
    //client.send(server.getName());
    client.send("\r\nCAP * LS :");
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
    std::string cap = popWd(caps);
    std::cout << "[capReq] caps:" << caps << std::endl;
    //client.send(server.getName());
    if (!cap.empty())
    {
        for (int i = 0; cap_tab[i].f; i++)
        {
            if (cap == cap_tab[i].header) {
                cap_tab[i].f(server, client, "");
            } else {
                client.send(":" + server.getName() + " CAP " + client.getNickname() + " NAK :" + cap + "\r\n");
            }
        }
    }
    client.send(":" + server.getName() + " 461 " + client.getNickname() + " CAP :Not enough parameters\r\n");
}

static void server_banner(Client &client)
{
    std::string message001 = ":ircSchoolProject 001 " + client.getNickname() + " :Welcome to the ircSchoolProject " + client.getNickname() + '\n';
    std::string message002 = ":ircSchoolProject 002 " + client.getNickname() + " :Your host is ircSchoolProject[10.13.4.10/6667], running version v1.0\n";
    std::string message003 = ":ircSchoolProject 003 " + client.getNickname() + " :This server was created Wed Jul 2025 at 12:00:00 EST\n";
    std::string message004 = ":ircSchoolProject 004 " + client.getNickname() + " ircSchoolProject v1.0 o itkol kl\n";
    std::string message005 = ":ircSchoolProject 005 " + client.getNickname() + " CHANMODES=o,k,l,it :are supported by this server\n";
    std::string messageMODE = ":" + client.getNickname() + " MODE " + client.getNickname() + " :+i\n";
    client.send(message001);
    client.send(message002);
    client.send(message003);
    client.send(message004);
    client.send(message005);
    client.send(messageMODE);
}

void capEnd(Server &server, Client &client, std::string caps) {
    (void)server;
    (void)caps;
    if (!client.hasFlag(LOGGED)) {
        if (client.getUsername() != "" && client.getNickname() != "" && client.hasFlag(PASSWD_OK)) {
            client.setFlag(LOGGED);
        }
        server_banner(client);
    }
}
