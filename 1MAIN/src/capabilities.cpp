//
// Created by n on 20/07/25.
//

#include "capabilities.hpp"

#include <string>

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
    if (client.hasCap("multi-prefix")) {
        client.resetCap("multi-prefix");
    } else {
        client.setCap("multi-prefix");
    }
}

void capLs(Server& server, Client& client, std::string args)
{
    (void)server;
    (void)args;
    client.send(server.getName());
    client.send("\r\nCAP * LS :");
    for (int i = 0; cap_tab[i].f; i++)
    {
        client.send(cap_tab[i].header);
        client.send(" ");
    }
    client.setFlag(LOG_IN);
    client.send("\n");
}

void capReq(Server& server, Client& client, std::string caps)
{
    (void)server;
    std::string cap = getHead(caps);
    std::cout << "[capReq] caps:" << caps << std::endl;
    //client.send(server.getName());
    while (!cap.empty())
    {
        for (int i = 0; cap_tab[i].f; i++)
        {
            if (cap == cap_tab[i].header) {
                cap_tab[i].f(server, client, "");
            } else {
                client.send("\n[capReq] : " + cap + "Invalid\r\n");
            }
            caps = getNextWds(caps);
            cap = getHead(caps);
        }
    }
}

void capEnd(Server &server, Client &client, std::string caps) {
    (void)server;
    (void)caps;
    if (client.getUsername() != "" && client.getNickname() != "") {
        client.resetFlag(LOG_IN);
        client.setFlag(LOGGED);
    }
    client.send(":ircSchoolProject 001 TestUser :Welcome to the ircSchoolProject TestUser\n");
    client.send(":ircSchoolProject 002 TestUser :Your host is ircSchoolProject[10.13.2.7/6667], running version 0.1\n");
    client.send(":ircSchoolProject 003 TestUser :This server was created Wed Jul 2025 at 12:00:00 EST\n");
    client.send(":ircSchoolProject 004 TestUser ircSchoolProject oiwszcrkfydnxbauglZCD biklmnopstveIrS bkloveI\n");
    client.send(":ircSchoolProject 005 TestUser SAFELIST ELIST=U CASEMAPPING=rfc1459 CHARSET=ascii NICKLEN=9\
        CHANNELLEN=50 TOPICLEN=160 ETRACE CPRIVMSG CNOTICE DEAF=D MONITOR=120 :are supported by this server\n");
    client.send(":ircSchoolProject 005 TestUser FNC ACCEPT=20 MAP TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:10,\
        NOTICE:10,ACCEPT:,MONITOR: :are supported by this server\n");
}
