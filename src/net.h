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

/* client */
typedef struct
{
  SOCKET sock;
  char id[BUF_SIZE];
}Client;

/* events */
typedef struct network_s network;
typedef void (*input_event_function)      (network *);
typedef void (*connection_event_function) (network *, Client *, char *);
typedef void (*deconnection_event_function) (network *, Client *);
typedef void (*message_event_function)    (network *, Client *, char *);

/* network */
struct network_s{
  SOCKET server;
  unsigned int max;              // max socket id (cf. select)
  Client *clients;               // array of clients
  unsigned int nb_clients;       // index for the array
  //unsigned int allocated_clients;

  // events
  input_event_function        input_event;
  connection_event_function   connection_event;
  deconnection_event_function deconnection_event;
  message_event_function      message_event;
};

/* windows compatibility */
static void init(void);
static void end(void);

/* public functions */
static network *new_network    (unsigned int port);
static void     free_network   (network *net);
static void     update_network (network *net);
static void     connect_network(network *net, 
				const char *address,
				const unsigned int port);
#endif
