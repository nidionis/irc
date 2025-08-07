/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:43:30 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/07 12:38:45 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by nidionis on 04/07/25.
//

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include <iostream>

# include "Client.hpp"

# define GETTER_SETTER(type, name, f_name) \
type get##f_name() const { return name; } \
void set##f_name(type value) { name = value; }

class Client;

class Channel {
private:
    std::string _name;
    std::string _key;
    std::string _topic;
    std::vector<Client> clients;
    std::vector<Client> operators;
    std::vector<char> modes;
public:
    Channel();
    Channel(Client &client, std::string &name);
    bool operator==(const Channel &other) const;
    Channel &operator=(const Channel &other);
    ~Channel();

    GETTER_SETTER(std::string, _name, Name)
    GETTER_SETTER(std::string, _key, Key)
    GETTER_SETTER(std::string, _topic, Topic)

    bool    isOperator(Client &client);
    void    setOperator(Client &client);
    void    delOperator(Client &client);

    bool    isClient(Client &client);
    void    setClient(Client &client);
    void    delClient(Client &client);

    void    setMode(char mode);
    void    delMode(char mode);
    bool    hasMode(char mode);

    void    spawn(std::string msg);
};

#endif //CHANNEL_HPP
