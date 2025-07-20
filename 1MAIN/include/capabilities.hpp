//
// Created by n on 20/07/25.
//

#ifndef CAPABILITIES_H
# define CAPABILITIES_H

// Forward declarations
class Server;
class Client;

#include <string>
#include "Handle.hpp"  // For s_cmd definition

void multiPrefix(Server &server, Client &client, std::string input);
void capLs(Server &server, Client &client, std::string args);
void capReq(Server &server, Client &client, std::string caps);

extern const struct s_cmd req[];

#endif //CAPABILITIES_H