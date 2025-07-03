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

//I'll provide a comprehensive list of attributes typically associated with an IRC (Internet Relay Chat) client:
//1. Basic Identification Attributes:
//
//- `nickname` (required): The unique identifier used in the chat
//- `username`: The actual username of the client
//- `realname` (full name): User's real or display name
//- `hostname`: Client's host/IP address
//
//1. Connection Attributes:
//
//- `socket_fd`: File descriptor for the client's socket connection
//- `server_address`: IP/address of the connected IRC server
//- `port`: Server connection port
//- `connection_time`: Timestamp of when the client connected
//- `is_connected`: Boolean flag indicating connection status
//- `connection_type`: (e.g., SSL, plain text)
//
//1. Chat-Related Attributes:
//
//- `channels`: List of channels the client has joined
//- `channel_modes`: User's modes in each channel
//- `away_status`: Whether the user is marked as "away"
//- `away_message`: Message displayed when user is away
//
//1. Authentication Attributes:
//
//- `user_id`: Unique identifier in the server
//- `password`: Authentication credential (hashed)
//- `authentication_status`: Logged in/not logged in
//- `registration_time`: When the user registered
//
//1. Client Metadata:
//
//- `client_version`: IRC client software version
//- `client_type`: (e.g., desktop, web, mobile)
//- `user_modes`: Global user modes (e.g., +i for invisible)
//
//1. Communication Attributes:
//
//- `ignored_users`: List of users the client has ignored
//- `message_history`: Recent private and channel messages
//- `last_active_time`: Timestamp of last user activity
//
//1. Advanced Attributes:
//
//- `ssl_certificate`: For secure connections
//- `geolocation`: User's geographic location
//- `preferred_encoding`: Text encoding preference
//- `timezone`: User's local time zone
//
//1. Permission and Access Attributes:
//
//- `is_operator`: Whether the user has operator status
//- `is_admin`: Administrative privileges
//- `access_level`: Permissions in channels or server


class Client
{
private:
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _hostname;
    //std::vector<Channels>	channels;

public:

	struct sockaddr_in	IPv4_client_sock_addr;
	int					fd_client_socket;
	socklen_t			client_addrlen;

	Client(void);
	~Client(void);

    void				setNickname(std::string buffer);
    std::string         &getNickname(void);
    void            	setUsername(std::string buffer);
    std::string         &getUsername(void);
    void                setRealname(std::string buffer);
    std::string         &getRealname(void);
    void                setHostname(std::string buffer);
    std::string         &getHostname(void);
    void				clientCleanup(void);
};

#endif //CLIENT_HPP
