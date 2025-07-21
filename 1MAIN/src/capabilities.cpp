//
// Created by n on 20/07/25.
//

#include "capabilities.hpp"

const struct s_cmd req[] = {
    {"multi-prefix",    &multiPrefix },
    {"",                NULL }
};

void multiPrefix(Server& server, Client& client, std::string input)
{
    (void)server;
    (void)client;
    (void)input;
    client.send("\n[multiPrefix] returns prefixes set in macro USER_PREFIXES (Client.hpp)\r\n");
    client.send("\r\n");
    for (int i = 0; USER_PREFIXES[i]; i++)
    {
        client.send(" ");
        client.send(std::string(1, USER_PREFIXES[i]));
    }
    client.send("\nyapuka !\r\n");
}

void capLs(Server& server, Client& client, std::string args)
{
    (void)server;
    (void)args;
    client.send("\r\nCAP * LS : ");
    for (int i = 0; req[i].f; i++)
    {
        client.send(" ");
        client.send(req[i].header);
    }
    client.send("\nCAP END");
    client.send("\n");
}

// as processCmd
//apply function in req[] if asked by client by CAP REQ request
// note : called by cmdCap
void capReq(Server& server, Client& client, std::string caps)
{
    (void)server;
    std::string cap = getHead(caps);
    std::cout << "[capReq] caps:" << caps << std::endl;
    while (!cap.empty())
    {
        for (int i = 0; req[i].f; i++)
        {
            if (cap == req[i].header) {
                req[i].f(server, client, "");
            } else {
                client.send("\n[capReq] : Invalid CAP req command\r\n");
            }
            caps = getNextWds(caps);
            cap = getHead(caps);
        }
    }
}
