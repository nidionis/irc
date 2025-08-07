/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_poll.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:43:56 by lahlsweh          #+#    #+#             */
/*   Updated: 2025/08/07 12:25:10 by lahlsweh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by nidionis on 02/07/25.
//

#include "Server.hpp"

volatile sig_atomic_t exit_program = false;

void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        std::cout << "\nSIGINT caught. Terminating program ..." << std::endl;
        exit_program = true;
        return ;
    }
}

void Server::pollLoop(void)
{
    poll_data p_data;

    std::signal(SIGINT, handle_signal);
    p_data.fd_nb = 1;
    p_data.err_check = 0;
    for (int i = 0; i < MAX_CONNECTIONS; ++i)
    {
        p_data.fds[i].fd = this->fd_server_socket;
        p_data.fds[i].events = POLLIN;
        p_data.fds[i].revents = 0;
    }
    while (true)
    {
        if (exit_program == true) { break ; }
        p_data.err_check = poll(p_data.fds, p_data.fd_nb, -1);
        if (p_data.err_check == -1)
        {
            if (errno == EINTR) { continue; }
            else { pollDataCleanup(&p_data); throw (std::runtime_error("poll() error")); }
        }
        for (p_data.i = 0; p_data.i < p_data.fd_nb; p_data.i++)
        {
            if (p_data.fds[p_data.i].revents & (POLLERR | POLLHUP | POLLNVAL))
            {
                std::cerr << "Error/hangup on fd: " << p_data.fds[p_data.i].fd << ". Closing." << std::endl;
                pollFailHandler(&p_data);
            }
            else if (p_data.fds[p_data.i].revents & POLLIN) { pollClientHandler(&p_data); }
        }
    }
    return;
}

void Server::pollFailHandler(poll_data* p_data)
{
    int bad_fd = p_data->fds[p_data->i].fd;

    close(bad_fd);
    for (size_t j = 0; j < this->vector_clients.size(); j++)
    {
        if (this->vector_clients[j].fd_client_socket == bad_fd)
        {
            this->vector_clients.erase(this->vector_clients.begin() + j);
            break;
        }
    }
    p_data->fds[p_data->i] = p_data->fds[p_data->fd_nb - 1];
    p_data->fds[p_data->i].events = 0;
    p_data->fds[p_data->i].revents = 0;
    p_data->fd_nb--;
    p_data->i--;
    return;
}

void Server::pollClientHandler(poll_data* p_data)
{
    if (p_data->fds[p_data->i].fd == this->fd_server_socket)
    {
        pollClientConnect(p_data);
    } else {
        try {
            pollClientRecv(p_data);
        } catch (const std::exception &err) {
            err.what();
        }
    }
    return ;
}

void	Server::pollClientConnect(poll_data* p_data)
{
    this->vector_clients.push_back(Client(this));
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
        if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
            throw (std::runtime_error("recv failed"));
            return ;
        } else {
            pollClientDisconnect(p_data);
            pollDataCleanup(p_data);
            throw (std::runtime_error("recv() error"));
        }
    } else if (recv_read == 0) {
        pollClientDisconnect(p_data);
    } else {
        buffer[recv_read] = '\0';
        handle(buffer, getClient(p_data->i));
        if (getClient(p_data->i).must_kill == true)
        {
            std::cout << "TMP TEST : CLIENT KILLED" << std::endl;
            pollClientDisconnect(p_data);
        }
        usleep(200);
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
