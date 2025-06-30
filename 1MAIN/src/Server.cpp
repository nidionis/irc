/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:38:24 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/06/30 12:48:55 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
#include "Server.hpp"
#include "Client.hpp"

Server::Server(void)
{
	memset(&this->IPv4_serv_sock_addr, 0, sizeof(this->IPv4_serv_sock_addr));
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
	this->IPv4_serv_sock_addr.sin_family = AF_INET;
	this->IPv4_serv_sock_addr.sin_port = htons(PORT);
	this->IPv4_serv_sock_addr.sin_addr.s_addr = INADDR_ANY;
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
	poll_data poll_data;
	
	poll_data.fd_i = 1;
	poll_data.err_check = 0;
	poll_data.fds[0].fd = this->fd_server_socket;
	poll_data.fds[0].events = POLLIN;
	while (true)
	{
		poll_data.err_check = poll(poll_data.fds, poll_data.fd_i, -1);
		if (poll_data.err_check == -1)
		{
			if (errno == EINTR)  { continue; }
			else { pollDataCleanup(&poll_data); throw (std::runtime_error("poll() error")); }
		}
		for (poll_data.i = 0; poll_data.i < poll_data.fd_i; poll_data.i++)
		{
			if (poll_data.fds[poll_data.i].revents & (POLLERR | POLLHUP | POLLNVAL))
				{ pollFailHandler(&poll_data); }
			else if (poll_data.fds[poll_data.i].revents & POLLIN)
			{
				pollClientHandler(&poll_data);
			}
		}
	}
	return;
}

void Server::pollFailHandler(poll_data* poll_data)
{
	std::cout << "Error/hangup on fd: " << poll_data->fds[poll_data->i].fd << ". Closing." << std::endl;
	close(poll_data->fds[poll_data->i].fd);
	poll_data->fds[poll_data->i] = poll_data->fds[poll_data->fd_i - 1];
	poll_data->fd_i--;
	poll_data->i--;
	return;
}

void Server::pollClientHandler(poll_data* poll_data)
{
	if (poll_data->fds[poll_data->i].fd == this->fd_server_socket)
		{ pollClientConnect(poll_data); }
	else
		{ pollClientRecv(poll_data); }
	return ;
}

void	Server::pollClientConnect(poll_data* poll_data)
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
		else { pollDataCleanup(poll_data); throw (std::runtime_error("accept() error")); }
	}
	if (fcntl(new_client.fd_client_socket, F_SETFL, O_NONBLOCK) == -1)
		{ pollDataCleanup(poll_data); throw (std::runtime_error("fcntl() error")); }
	if (poll_data->fd_i < MAX_CONNECTIONS)
	{
		poll_data->fds[poll_data->fd_i].fd = new_client.fd_client_socket;
		poll_data->fds[poll_data->fd_i].events = POLLIN;
		poll_data->fd_i++;
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

void	Server::pollClientRecv(poll_data* poll_data)
{
	ssize_t	recv_read;

	memset(buffer, 0, BUFFER_SIZE);
	recv_read = recv(poll_data->fds[poll_data->i].fd, buffer, (BUFFER_SIZE - 1), 0);
	if (recv_read == -1)
	{
		if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{ std::cerr << "error: Client read failed." << std::endl; return ; }
		else { pollClientDisconnect(poll_data); pollDataCleanup(poll_data); throw (std::runtime_error("recv() error")); }
	}
	else if (recv_read == 0)
		{ pollClientDisconnect(poll_data); }
	else
	{
		if (recv_read < BUFFER_SIZE) { buffer[recv_read] = '\0'; }
		else { buffer[BUFFER_SIZE - 1] = '\0'; }
		std::cout << "Received from fd " << poll_data->fds[poll_data->i].fd << ": " << buffer << std::flush;
	}
	return ;
}

void	Server::pollClientDisconnect(poll_data* poll_data)
{
	std::cout << "Client on fd " << poll_data->fds[poll_data->i].fd << " disconnected." << std::endl;
	for (size_t i = 0; i < this->vector_clients.size(); i++)
	{
		if (this->vector_clients[i].fd_client_socket == poll_data->fds[poll_data->i].fd)
		{
			this->vector_clients.erase(this->vector_clients.begin() + i);
			break ;
		}
	}
	close(poll_data->fds[poll_data->i].fd);
	poll_data->fds[poll_data->i] = poll_data->fds[poll_data->fd_i - 1];
	poll_data->fd_i--;
	poll_data->i--;
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

/*static void	configureSockAddr(struct sockaddr_in &sockAddr, int port);
static bool	unBlockSocket(int fd);
static bool	ft_setsockopt(int server_fd);


struct pollfd Server::initSocket() throw(std::runtime_error)
{
	struct pollfd pfd;
	while ((pfd.fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == 0)
	{
		continue ;
	}
	if (pfd.fd < 0)
	{
		throw std::runtime_error("Socket creation failed");
	}
	if (!ft_setsockopt(pfd.fd))
	{
		throw std::runtime_error("Setting socket failed");
	}
	pfd.events = POLLIN | POLLHUP;
	_sockets[_nb_socket] = pfd;
	_nb_socket++;
	return pfd;
}

bool Server::listenUp(int i_socket)
{
	if (i_socket >= MAX_SOCKET)
	{
		throw std::runtime_error("Too many sockets");
	}
	struct pollfd &pfd = _sockets[i_socket];
	struct sockaddr_in &sockAddr = _addresses[i_socket];
	configureSockAddr(sockAddr, _port);
	unBlockSocket(pfd.fd);
	if (bind(pfd.fd, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) < 0)
	{
		throw std::runtime_error("Bind failed");
	}
	if (listen(pfd.fd, MAX_SOCKET) < 0)
	{
		throw std::runtime_error("Listen failed");
	}
	std::cout << "Server listening on port " << _port << std::endl;
	return true;
}

Client *Server::connection(int i_socket)
{
	int fd;
	int addrlen = sizeof(struct sockaddr_in);
	Client *client = new Client();
	while ((fd = accept(_sockets[i_socket].fd, (struct sockaddr *)&_addresses[i_socket], (socklen_t*)&addrlen)) < 0)
	{
		continue;
	}
	std::cout << "Client connected." << std::endl;
	client->setFd(fd);
	return client;
}

void Server::pollRun()
{
	int activity;
	activity = ::poll(_sockets, _nb_socket, -1);
	if (activity < 0) {
		throw std::runtime_error("Poll error");
	}
}

int Server::receivingSocket()
{
	for (int j = 0; j < _nb_socket; j++)
	{
		printf("revents = %d\n", _sockets[j].revents);
		if (_sockets[j].revents & POLLIN)
		{
			return j;
		}
	}
	throw std::runtime_error("Nothing to receive");
}

struct sockaddr_in &Server::getSockAddr(int i_socket)
{
	return _addresses[i_socket];
}

struct pollfd &Server::getPfd(int i_socket)
{
	return _sockets[i_socket];
}

int Server::getFd(int i_socket)
{
	return _sockets[i_socket].fd;
}

static bool ft_setsockopt(int server_fd)
{
	int reuseAddrFlag = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseAddrFlag, sizeof(reuseAddrFlag)) < 0)
	{
		return false;
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_KEEPALIVE, &reuseAddrFlag, sizeof(reuseAddrFlag)) < 0)
	{
		return false;
	}
	return true;
}

static bool unBlockSocket(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
	{
		throw std::runtime_error("Failed to get socket flags");
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		throw std::runtime_error("Failed to set non-blocking mode");
	}
	return true;
}

static void configureSockAddr(struct sockaddr_in &sockAddr, int port)
{
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = INADDR_ANY;
	sockAddr.sin_port = htons(port);
	return ;
}*/
