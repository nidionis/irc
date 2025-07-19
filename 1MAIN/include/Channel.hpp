//
// Created by nidionis on 04/07/25.
//

#ifndef IRC_CHANNEL_H
#define IRC_CHANNEL_H
#include <Client.hpp>

class Channel {
private:
    std::vector<Client> clients;
    std::vector<Client> admins;
public:
    std::string name;
    Channel();
    Channel(Client &client, std::string &name);
    bool operator==(const Channel &other) const;
    ~Channel();

    bool    isAdmin(Client &client);
    bool    isClient(Client &client);
    void    setAdmin(Client &client);
    void    setClient(Client &client);
};

#endif //IRC_CHANNEL_H
