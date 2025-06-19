#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <cstdlib>
# include <cerrno>
# include <csignal>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <fcntl.h>
# include <poll.h>
# include <unistd.h>

#define BUFF_SIZE 1024
#define ERROR -1
#define OK 0

#include <Channel.h>
#include <Server.h>
#include <Client.h>
#include <Message.h>
#include <Handler.h>

// Function prototypes
int testing(int argc, char **argv);

#endif // IRCSERV_HPP
