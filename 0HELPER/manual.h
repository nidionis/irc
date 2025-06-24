

typedef uint32_t		socklen_t;
typedef long			off_t;
typedef unsigned long	nfds_t;
typedef uint32_t		in_addr_t;

typedef struct	sockaddr
{
	sa_family_t	sa_family;   // Address family (e.g., AF_INET for IPv4)
	char		sa_data[14]; // Socket address data
};

typedef struct	sigaction
{
	void		(*sa_handler)(int);                        // Signal handler function
	void		(*sa_sigaction)(int, siginfo_t *, void *); // Signal handler function with additional info
	sigset_t	sa_mask;   // Additional set of signals to be blocked during the execution of the signal handler
	int			sa_flags;                                  // Special flags to affect the behavior of the signal
};

typedef struct	protoent
{
	char	*p_name;     // Official name of the protocol
	char	**p_aliases; // Alias list for the protocol
	int		p_proto;     // Protocol number
};

typedef struct	hostent
{
	char	*h_name;	   // Official name of the host
	char	**h_aliases;   // Alias list for the host
	int		h_addrtype;    // Host address type (e.g., AF_INET)
	int		h_length;      // Length of the address
	char	**h_addr_list; // List of addresses for the host
};

typedef struct	stat
{
	dev_t		st_dev;      // ID of device containing file
	ino_t		st_ino;      // Inode number
	mode_t		st_mode;     // File type and mode
	nlink_t		st_nlink;    // Number of hard links
	uid_t		st_uid;	     // User ID of owner
	gid_t		st_gid;	     // Group ID of owner
	dev_t		st_rdev;     // Device ID (if special file)
	off_t		st_size;     // Total size, in bytes
	blksize_t	st_blksize;  // Block size for filesystem I/O
	blkcnt_t	st_blocks;   // Number of 512B blocks allocated
	time_t		st_atime;    // Time of last access
	time_t		st_mtime;    // Time of last modification
	time_t		st_ctime;    // Time of last status change
};

typedef struct	pollfd
{
	int		fd;	     // File descriptor
	short	events;  // Requested events to monitor
	short	revents; // Returned events that occurred
};

typedef struct	addrinfo
{
	int				ai_flags;      // Input flags
	int				ai_family;     // Address family for socket
	int				ai_socktype;   // Socket type
	int				ai_protocol;   // Protocol for socket
	socklen_t		ai_addrlen;    // Length of socket address
	struct sockaddr	*ai_addr;      // Socket address for socket
	char			*ai_canonname; // Canonical name for service location
	struct addrinfo	*ai_next;      // Pointer to next structure in linked list
};

typedef struct	in_addr
{
	in_addr_t	s_addr; // IPv4 address
};

#include <sys/socket.h>
int				socket(int domain, int type, int protocol);
int				setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
int				getsockname(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
int				bind(int socket, const struct sockaddr *address, socklen_t address_len);
int				connect(int socket, const struct sockaddr *address, socklen_t address_len);
int				listen(int socket, int backlog);
int				accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
ssize_t			send(int socket, const void *buffer, size_t length, int flags);
ssize_t			recv(int socket, void *buffer, size_t length, int flags);

#include <arpa/inet.h>
uint32_t		htonl(uint32_t hostlong);
uint16_t		htons(uint16_t hostshort);
uint32_t		ntohl(uint32_t netlong);
uint16_t		ntohs(uint16_t netshort);

#include <signal.h>
void			(*signal(int sig, void (*func)(int)))(int);
int				sigaction(int sig, const struct sigaction *restrict act, struct sigaction *restrict oact);

#include <netdb.h>
struct protoent	*getprotobyname(const char *name);
struct hostent	*gethostbyname(const char *name);

#include <unistd.h>
int				close(int fildes);
off_t			lseek(int fildes, off_t offset, int whence);

#include <sys/stat.h>
int				fstat(int fildes, struct stat *buf);

#include <fcntl.h>
int				fcntl(int fildes, int cmd, ...);

#include <poll.h>
int				poll(struct pollfd fds[], nfds_t nfds, int timeout);

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
int				getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
void			freeaddrinfo(struct addrinfo *res);

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
in_addr_t		inet_addr(const char *cp);
char			*inet_ntoa(struct in_addr in);


/*
Create a socket
Bind it to a port (like 6667)
Listen for connections
Accept a connection
Receive data and send replies
*/
