#ifndef NET_H_
#define NET_H_

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not defined for this platform

#endif

#define CRLF		"\r\n"
#define NB_CLIENTS 	100

#define BUF_SIZE	1024


typedef struct
{
  SOCKET sock;
  char id[BUF_SIZE];
}Client;

typedef struct{
  SOCKET server;
  unsigned int max;              // max socket id (cf. select)
  Client *clients;               // array of clients
  unsigned int nb_clients;       // index for the array
  //unsigned int allocated_clients;
} network;

static void init(void);
static void end(void);


network *new_network   (unsigned int port);
void     free_network  (network *net);
void     update_network(network *net);

#endif
