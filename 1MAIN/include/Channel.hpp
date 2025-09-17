/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:29:51 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/09/17 11:45:12 by lahlsweh         ###   ########.fr       */
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

class Channel
{
private:
	std::string			_name;
	std::string			_key;
	std::string			_topic;
	std::vector<Client>	_clients;
	std::vector<Client>	_operators;
	std::vector<char>	_modes;

public:
	int					max_users;
	Channel(void);
	Channel(Client& client, std::string& name);
	Channel	&operator=(const Channel& other);
	~Channel(void);

	bool	operator==(const Channel& other) const;

	GETTER_SETTER(std::string, _name, Name)
	GETTER_SETTER(std::string, _key, Key)
	GETTER_SETTER(std::string, _topic, Topic)

	bool	isOperator(Client& client);
	void	setOperator(Client& client);
	void	delOperator(Client& client);

	bool	isClient(Client& client);
	void	setClient(Client& client);
	void	delClient(Client& client);

	void	setMode(char mode);
	void	delMode(char mode);
	bool	hasMode(char mode);

	void	spawn(std::string msg);
	bool	isEmpty();
};

#endif //CHANNEL_HPP
