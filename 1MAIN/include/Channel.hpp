//
// Created by nidionis on 04/07/25.
//

#ifndef IRC_CHANNEL_H
#define IRC_CHANNEL_H
#include <Client.hpp>
# define MAX_CHANNELS 2
# define CHAN_OP "itkol"
# define INVITE_ONLY    0
# define TOPIC_RESTRICT 1
# define KEY            2
# define OP             3
# define LIMIT          4

class Channel {
private:
    std::vector<Client> clients;
    std::vector<Client> admins;
    std::vector<std::string> op;
public:
    std::string name;
    Channel();
    Channel(Client &client, std::string &name);
    bool operator==(const Channel &other) const;
    Channel &operator=(const Channel &other);
    ~Channel();

    bool    isAdmin(Client &client);
    bool    isClient(Client &client);
    void    setAdmin(Client &client);
    void    setClient(Client &client);
    void    delClient(Client &client);
    void    setOp(std::string op);
    void    delOp(std::string op);
    bool    hasOp(std::string op);
};

#endif //IRC_CHANNEL_H
