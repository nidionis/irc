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
    size_t i = 0;
    while (cap_tab[i].header)
    {
        if (cap == cap_tab[i].header)
            return true;
        i++;
    }
    return false;
}

//void (*getCap(const std::string& cap))(Server&, Client&, std::string)
//{
//    size_t i = 0;
//    while (cap_tab[i].header)
//    {
//        if (cap == cap_tab[i].header)
//            return cap_tab[i].f;
//        i++;
//    }
//    return NULL;
//}

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
    client.setFlag("loging");
}

// as processCmd
//apply function in cap_tab[] if asked by client by CAP REQ request
// note : called by cmdCap
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
        client.resetFlag("loging");
        client.setFlag("logged");
    }
}
