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
    client.send("\r\nCAP * LS : ");
    for (int i = 0; cap_tab[i].f; i++)
    {
        client.send(" ");
        client.send(cap_tab[i].header);
    }
    client.setFlag(LOG_IN);
    client.send("\n");
}

void capReq(Server& server, Client& client, std::string caps)
{
    (void)server;
    std::string cap = getHead(caps);
    std::cout << "[capReq] caps:" << caps << std::endl;
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
}