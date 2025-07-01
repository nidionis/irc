/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:38:24 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/30 13:46:50 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
#include "Server.hpp"
#include "Client.hpp"

Server::Server(void)
{
	memset(&this->IPv4_serv_sock_addr, 0, sizeof(this->IPv4_serv_sock_addr));
	this->IPv4_serv_sock_addr.sin_family = AF_INET;
	this->IPv4_serv_sock_addr.sin_port = htons(PORT);
	this->IPv4_serv_sock_addr.sin_addr.s_addr = INADDR_ANY;
	this->fd_server_socket = -1;
	memset(this->buffer, 0, BUFFER_SIZE);
	this->vector_clients.empty();
	return ;
}

Server::~Server(void)
{
	return ;
}

void	Server::serverSetup(void)
{
	initServerSocket();
	setServerSockopt();
	bindServerSocket();
	listenServerSocket();
	std::cout << "Server listening on port " << PORT << "..." << std::endl;
	return ;
}

void				Server::initServerSocket(void)
{
	this->fd_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->fd_server_socket == -1)
		{ throw (std::runtime_error("socket() error")); }
	if (fcntl(this->fd_server_socket, F_SETFL, O_NONBLOCK) == -1)
		{ throw (std::runtime_error("fcntl() error")); }
	return ;
}

void				Server::setServerSockopt(void)
{
	int	opt_toggle = 1;
	
	if (setsockopt(this->fd_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt_toggle, sizeof(opt_toggle)) == -1)
		{ throw (std::runtime_error("setsockopt() SO_REUSEADDR error")); }
	if (setsockopt(this->fd_server_socket, SOL_SOCKET, SO_KEEPALIVE, &opt_toggle, sizeof(opt_toggle)) == -1)
		{ throw (std::runtime_error("setsockopt() SO_KEEPALIVE error")); }
	return ;
}

void				Server::bindServerSocket(void)
{
	if (bind(this->fd_server_socket, (struct sockaddr *)&this->IPv4_serv_sock_addr, sizeof(this->IPv4_serv_sock_addr)) == -1)
		{ throw (std::runtime_error("bind() error")); }
	return ;
}

void				Server::listenServerSocket(void)
{
	if (listen(this->fd_server_socket, QUEUE_SIZE) == -1)
		{ throw (std::runtime_error("listen() error")); }
	return ;
}

void Server::pollLoop(void)
{
	poll_data p_data;
	
	p_data.fd_nb = 1;
	p_data.err_check = 0;
	p_data.fds[0].fd = this->fd_server_socket;
	p_data.fds[0].events = POLLIN;
	while (true)
	{
		p_data.err_check = poll(p_data.fds, p_data.fd_nb, -1);
		if (p_data.err_check == -1)
		{
			if (errno == EINTR) { continue; }
			else { pollDataCleanup(&p_data); throw (std::runtime_error("poll() error")); }
		}
		for (p_data.i = 0; p_data.i < p_data.fd_nb; p_data.i++)
		{
			if (p_data.fds[p_data.i].revents & (POLLERR | POLLHUP | POLLNVAL))
				{ pollFailHandler(&p_data); }
			else if (p_data.fds[p_data.i].revents & POLLIN)
				{ pollClientHandler(&p_data); }
		}
	}
	return;
}

void Server::pollFailHandler(poll_data* p_data)
{
	std::cout << "Error/hangup on fd: " << p_data->fds[p_data->i].fd << ". Closing." << std::endl;
	close(p_data->fds[p_data->i].fd);
	p_data->fds[p_data->i] = p_data->fds[p_data->fd_nb - 1];
	p_data->fd_nb--;
	p_data->i--;
	return;
}

void Server::answerClient(poll_data *p_data) { //, Request &request) {
    try {
        Client &client = this->getClient(p_data->i);
        std::string msg = "A msg from server\n";
        //std::string msg = request.answer();
        try {
            sendClient(client, msg);
        } catch (const std::exception err) {
            std::cout << err.what() << std::endl;
            return ;
        }
    } catch (const std::exception err) {
        std::cout << err.what() << std::endl;
        return ;
    }
}

void Server::pollClientHandler(poll_data* p_data)
{
	if (p_data->fds[p_data->i].fd == this->fd_server_socket)
		{ pollClientConnect(p_data); }
	else
	{
		pollClientRecv(p_data);
        std::string request = "/helloWorld";
        answerClient(p_data);//, request);
	}
	return ;
}

void	Server::pollClientConnect(poll_data* p_data)
{
	this->vector_clients.push_back(Client());
	Client&	new_client = this->vector_clients.back();
	
	new_client.fd_client_socket = accept(this->fd_server_socket,
		(struct sockaddr *)&new_client.IPv4_client_sock_addr, &new_client.client_addrlen);
	if (new_client.fd_client_socket == -1)
	{
		this->vector_clients.pop_back();
		if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{ std::cerr << "error: Client connection failed." << std::endl; return ; }
		else { pollDataCleanup(p_data); throw (std::runtime_error("accept() error")); }
	}
	if (fcntl(new_client.fd_client_socket, F_SETFL, O_NONBLOCK) == -1)
		{ pollDataCleanup(p_data); throw (std::runtime_error("fcntl() error")); }
	if (p_data->fd_nb < MAX_CONNECTIONS)
	{
		p_data->fds[p_data->fd_nb].fd = new_client.fd_client_socket;
		p_data->fds[p_data->fd_nb].events = POLLIN;
		p_data->fd_nb++;
		std::cout << "Client connected: " << inet_ntoa(new_client.IPv4_client_sock_addr.sin_addr)
				  << ':' << ntohs(new_client.IPv4_client_sock_addr.sin_port) << std::endl;
	}
	else
	{
		std::cout << "Error: MAX_CONNECTIONS (" << MAX_CONNECTIONS << ") reached." << std::endl;
		close(new_client.fd_client_socket);
		new_client.fd_client_socket = -1;
	}
	return ;
}

void	Server::pollClientRecv(poll_data* p_data)
{
	ssize_t	recv_read;

	memset(buffer, 0, BUFFER_SIZE);
	recv_read = recv(p_data->fds[p_data->i].fd, buffer, (BUFFER_SIZE - 1), 0);
	if (recv_read == -1)
	{
		if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{ std::cerr << "error: Client read failed." << std::endl; return ; }
		else { pollClientDisconnect(p_data); pollDataCleanup(p_data); throw (std::runtime_error("recv() error")); }
	}
	else if (recv_read == 0)
		{ pollClientDisconnect(p_data); }
	else
	{
		if (recv_read < BUFFER_SIZE) { buffer[recv_read] = '\0'; }
		else { buffer[BUFFER_SIZE - 1] = '\0'; }
		std::cout << "Received from fd " << p_data->fds[p_data->i].fd << ": " << buffer << std::flush;
	}
	return ;
}

void	Server::pollClientDisconnect(poll_data* p_data)
{
	std::cout << "Client on fd " << p_data->fds[p_data->i].fd << " disconnected." << std::endl;
	for (size_t i = 0; i < this->vector_clients.size(); i++)
	{
		if (this->vector_clients[i].fd_client_socket == p_data->fds[p_data->i].fd)
		{
			this->vector_clients.erase(this->vector_clients.begin() + i);
			break ;
		}
	}
	close(p_data->fds[p_data->i].fd);
	p_data->fds[p_data->i] = p_data->fds[p_data->fd_nb - 1];
	p_data->fd_nb--;
	p_data->i--;
	return;
}

void				Server::serverCleanup(void)
{
	for (size_t i = 0; i < this->vector_clients.size(); i++)
		{ this->vector_clients[i].clientCleanup(); }
	this->vector_clients.empty();
	if (this->fd_server_socket != -1)
	{
		close(this->fd_server_socket);
		this->fd_server_socket = -1;
	}
	memset(&this->IPv4_serv_sock_addr, 0, sizeof(this->IPv4_serv_sock_addr));
	memset(this->buffer, 0, BUFFER_SIZE);
	return ;
}

ssize_t 		Server::sendClient(Client &cli, std::string &msg) {
    int byte_sent;
    int flags = MSG_DONTWAIT; // | MSG_NOSIGNAL;
    byte_sent = send(cli.fd_client_socket, msg.c_str(), msg.size(), flags);
    if (byte_sent < 0) {
        throw (std::runtime_error("sending client error"));
    }
    return byte_sent;
}

Client&	Server::getClient(int i) {
    for (std::vector<Client>::iterator it = vector_clients.begin(); it != vector_clients.end(); ++it) {
        if (!--i) { //i comes from p_data->i, which start at 1 (and i'ts dangerous)
            return *it;
        }
    }
    throw (std::runtime_error("client not found"));
}
