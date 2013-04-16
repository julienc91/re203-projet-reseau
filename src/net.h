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
#define CONNECTION_ERROR -1


/* * * * client * * * */
typedef struct
{
  SOCKET sock;
  char id[BUF_SIZE];
}Client;

/* * * * events * * * */
typedef struct network_s network;
typedef void (*input_event_function)      (network *, char *);
typedef void (*connection_event_function) (network *, Client *, char *);
typedef void (*disconnection_event_function) (network *, Client *);
typedef void (*message_event_function)    (network *, Client *, char *);

/* * * * network * * * */
struct network_s{
  SOCKET server;
  unsigned int max;              // max socket id (cf. select)
  Client *clients;               // array of clients
  unsigned int nb_clients;       // index for the array
  //unsigned int allocated_clients;

  // events
  input_event_function        input_event;
  connection_event_function   connection_event;
  disconnection_event_function disconnection_event;
  message_event_function      message_event;
};

/* * * * windows compatibility * * * */
static void init(void);
static void end(void);

/* * * * public functions * * * */
static network *network_open   (unsigned int port);
static void     network_close  (network *net);
static void     network_update (network *net);
static Client  *network_connect(network *net, 
				const char *address,
				const unsigned int port);
static void     network_send   (Client *, const char *message);
#endif
