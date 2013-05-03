#ifndef NET_H_
#define NET_H_

#include "sock.h"
#include "client.h"

#define NB_CLIENTS 	100


#define CONNECTION_ERROR -1


/* * * * events * * * */
typedef struct network_s network;
typedef void (*input_event_function)      (network *, char *);
typedef void (*connection_event_function) (network *, Client *, char *);
typedef void (*disconnection_event_function) (network *, Client *);
typedef void (*message_event_function)    (network *, Client *, char *);

/* * * * network * * * */
struct network_s{
  short status; // opened / closed
  SOCKET server;
  int max;              // max socket id (cf. select)
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
void     network__send       (Client *, const char *message);
void     network__broadcast  (network *net, const char *message);



#endif
