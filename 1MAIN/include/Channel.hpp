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
    Channel();
    Channel(Client &client);
    ~Channel();

    bool    isAdmin(Client &client);
    bool    isClient(Client &client);
    void    setAdmin(Client &client);
    void    setClient(Client &client);
};


#endif //IRC_CHANNEL_H
