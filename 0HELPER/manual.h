typedef unsigned long		size_t;
typedef unsigned long long	ssize_t;
typedef unsigned int		socklen_t;

typedef struct	sockaddr
{
	unsigned short	sa_family;		// Common data: address family and length.
	char			sa_data[14];	// Address data.
};

#include <sys/socket.h>

int					socket(int domain, int type, int protocol);
int					setsockopt(int socket, int level, int option_name, const void* option_value, socklen_t option_len);
int					getsockname(int socket, struct sockaddr* address, socklen_t* address_len);
int					getsockname(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
int					bind(int socket, const struct sockaddr* address, socklen_t address_len);
int					connect(int socket, const struct sockaddr* address, socklen_t address_len);
int					listen(int socket, int backlog);
int					accept(int socket, struct sockaddr* address, socklen_t* address_len);
ssize_t				send(int socket, const void* buffer, size_t length, int flags);
ssize_t				recv(int socket, void* buffer, size_t length, int flags);

/*-----------------------------------------------------------------------------------------------------------------------*/

#include <arpa/inet.h>

uint32_t			htonl(uint32_t hostlong);
uint16_t			htons(uint16_t hostshort);
uint32_t			ntohl(uint32_t netlong);
uint16_t			ntohs(uint16_t netshort);

/*-----------------------------------------------------------------------------------------------------------------------*/

#include <signal.h>

void				(*signal(int sig, void (*func)(int)))(int);
int					sigaction(int sig, const struct sigaction* act, struct sigaction* oact);

/*-----------------------------------------------------------------------------------------------------------------------*/

typedef char*	h_addr;

// Description of data base entry for a single service.
typedef struct	protoent
{
	char*	p_name;		// Official protocol name.
	char**	p_aliases;	// Alias list.
	int		p_proto;		// Protocol number.
};

// Description of data base entry for a single host.
typedef struct	hostent
{
	char*	h_name;			// Official name of host.
	char**	h_aliases;		// Alias list.
	int		h_addrtype;		// Host address type.
	int		h_length;		// Length of address.
	char**	h_addr_list;	// List of addresses from name server.
	h_addr	h_addr_list[0];	// Address, for backward compatibility.
};

#include <netdb.h>

struct protoent*	getprotobyname(const char* name);
struct hostent*		gethostbyname(const char* name);

/*-----------------------------------------------------------------------------------------------------------------------*/

typedef long	off_t;

#include <unistd.h>

int					close(int fildes);
off_t				lseek(int fildes, off_t offset, int whence);

/*-----------------------------------------------------------------------------------------------------------------------*/

typedef struct stat
{
	unsigned long	st_dev;			// Device.
	unsigned long	st_ino;			// File serial number.
	unsigned long	st_nlink;		// Link count.
	unsigned int	st_mode;		// File mode.
	unsigned int	st_uid;			// User ID of the file's owner.
	unsigned int	st_gid;			// Group ID of the file's group.
	unsigned long	st_rdev;		// Device number, if device.
	long			st_size;		// Size of file, in bytes.
	long			st_blksize;		// Optimal block size for I/O.
	long			st_blocks;		// Number 512-byte blocks allocated.
	long			st_atime;		// Time of last access.
	unsigned long	st_atimensec;	// Nscecs of last access.
	long			st_mtime;		// Time of last modification.
	unsigned long	st_mtimensec;	// Nsecs of last modification.
	long			st_ctime;		// Time of last status change.
	long			st_ctimensec;	// Nsecs of last status change.
	long			__glibc_reserved[3];
};

#include <sys/stat.h>

int					fstat(int fildes, struct stat* buf);

/*-----------------------------------------------------------------------------------------------------------------------*/

#include <fcntl.h>

int					fcntl(int fildes, int cmd, ...);

/*-----------------------------------------------------------------------------------------------------------------------*/

// Type used for the number of file descriptors.
typedef unsigned long int	nfds_t;

// Data structure describing a polling request.
typedef struct	pollfd
{
	int			fd;			// File descriptor to poll.
	short int	events;		// Types of events poller cares about.
	short int	revents;	// Types of events that actually occurred.
};

#include <poll.h>
int					poll(struct pollfd fds[], nfds_t nfds, int timeout);

/*-----------------------------------------------------------------------------------------------------------------------*/

typedef unsigned int		socklen_t;

typedef struct	sockaddr
{
	unsigned short	sa_family;		// Common data: address family and length.
	char			sa_data[14];	// Address data.
};

// Structure to contain information about address of a service provider.
struct addrinfo
{
	int					ai_flags;		// Input flags.
	int					ai_family;		// Protocol family for socket.
	int					ai_socktype;	// Socket type.
	int					ai_protocol;	// Protocol for socket.
	socklen_t			ai_addrlen;		// Length of socket address.
	struct sockaddr*	ai_addr;		// Socket address for socket.
	char*				ai_canonname;	// Canonical name for service location.
	struct addrinfo*	ai_next;		// Pointer to next in list.
};

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int					getaddrinfo(const char* node, const char* service, const struct addrinfo* hints, struct addrinfo** res);
void				freeaddrinfo(struct addrinfo* res);

/*-----------------------------------------------------------------------------------------------------------------------*/

// Internet address.
typedef uint32_t	in_addr_t;

struct in_addr
{
	in_addr_t	s_addr;
};

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

in_addr_t			inet_addr(const char* cp);
char*				inet_ntoa(struct in_addr in);

/*-----------------------------------------------------------------------------------------------------------------------*/
