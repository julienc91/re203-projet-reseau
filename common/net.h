#ifndef NET_H_
#define NET_H_

#include "sock.h"
#include "client.h"

#define NB_CLIENTS 	100


#define NETWORK__CONNECTION_ERROR -1
#define NETWORK__SEND_ERROR       -2

/* * * * events * * * */
typedef struct network_s network;
typedef void (*input_event_function)      (network *, char *);
typedef void (*connection_event_function) (network *, Client *, char *);
typedef void (*disconnection_event_function) (network *, Client *);
typedef void (*message_event_function)    (network *, Client *, char *);

/* * * * network * * * */
/**
 * \struct network
 * \brief Network object
 */
struct network_s{
  short status; // opened / closed
  SOCKET server;
  int max;              // max socket id (cf. select)
  Client *clients;               // array of clients
  unsigned int nb_clients;       // index for the array
  //unsigned int allocated_clients;

  // events
  /*! \brief Event (function pointer) raised when the input has been used */
  input_event_function        input_event;
  /*! \brief Event (function pointer) raised when a new client is connected */
  connection_event_function   connection_event;
  /*! \brief Event (function pointer) raised when a client disconnects */
  disconnection_event_function disconnection_event;
  /*! \brief Event (function pointer) raised when a client sends a message */
  message_event_function      message_event;
};

/* * * * windows compatibility * * * */
void init(void);
void end(void);

/* * * * public functions * * * */
network *network__open       (unsigned int port);
void     network__close      (network *net);
int      network__is_opened  (network *net);
void     network__free       (network *net);
void     network__update     (network *net);
Client  *network__connect    (network *net,
			     const char *address,
			     const unsigned int port);
void     network__disconnect (network *net, Client *c);
int      network__send       (Client *, const char *message);
int      network__broadcast  (network *net, const char *message);
void     network__debug      (network *net);


#endif
