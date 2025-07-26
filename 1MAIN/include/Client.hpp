/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:58:51 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/30 13:40:03 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <main.hpp>
# define LOG_IN "log_IN"
# define LOGGED "logged"

class Client
{
private:
    Server* server;
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _hostname;
    std::vector<Channel> channels;
    std::vector<std::string> capabilities;
    std::vector<std::string> flags;
    bool logged[MAX_CHANNELS + 1];

public:
    GETTER_SETTER(std::string, _nickname, Nickname)
    GETTER_SETTER(std::string, _username, Username)
    GETTER_SETTER(std::string, _realname, Realname)
    GETTER_SETTER(std::string, _hostname, Hostname)

    struct sockaddr_in IPv4_client_sock_addr;
    int fd_client_socket;
    socklen_t client_addrlen;

    Client(void);
    Client(Server* server);
    ~Client(void);
    bool operator==(const Client& other) const;
    void clientCleanup(void);

    Channel* newChannel(std::string& name);
    void delChannel(Channel& channel);
    ssize_t send(std::string msg);
    void setCap(const std::string& cap);
    void resetCap(const std::string& cap);
    bool hasCap(const std::string& cap);
    void setFlag(const std::string& cap);
    void resetFlag(const std::string& cap);
    bool hasFlag(const std::string& cap);
    ssize_t send_banner(std::string line);
    void setLog();
};

//std::ostream& operator<<(std::ostream& os, const Client& client);

#endif //CLIENT_HPP
