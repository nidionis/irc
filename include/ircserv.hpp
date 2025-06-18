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

#include <Channel.h>
#include <Client.h>
#include <Server.h>
#include <Message.h>
#include <Handler.h>

#define BUFF_SIZE 1024
#define ERROR -1
#define OK 0

// Function prototypes
int testing(int argc, char **argv);

#endif // IRCSERV_HPP
