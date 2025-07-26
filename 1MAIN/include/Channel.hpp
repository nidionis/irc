//
// Created by nidionis on 04/07/25.
//

#ifndef IRC_CHANNEL_H
#define IRC_CHANNEL_H
# include "main.hpp"
# define CHAN_OP "itkol"
# define INVITE_ONLY    0
# define TOPIC_RESTRICT 1
# define KEY            2
# define OP             3
# define LIMIT          4
# define TOPIC_MAX_LEN  2048

# define OPERATOR_OP "%@"
# define HALFOP 0
# define CHANOP 1

class Channel {
private:
    std::string _name;
    std::string _key;
    std::string _topic;
    std::vector<Client> clients;
    std::vector<Client> admins;
    std::vector<std::string> op;
public:
    Channel();
    Channel(Client &client, std::string &name);
    bool operator==(const Channel &other) const;
    Channel &operator=(const Channel &other);
    ~Channel();

    GETTER_SETTER(std::string, _name, Name)
    GETTER_SETTER(std::string, _key, Key)
    GETTER_SETTER(std::string, _topic, Topic)

    bool    isAdmin(Client &client);
    bool    isClient(Client &client);
    void    setAdmin(Client &client);
    void    setClient(Client &client);
    void    delClient(Client &client);

    void    setOp(std::string op);
    void    delOp(std::string op);
    bool    hasOp(std::string op);

    void    spawn(std::string msg);
};

#endif //IRC_CHANNEL_H
