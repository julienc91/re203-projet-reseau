#ifndef CLIENT_H_
#define CLIENT_H_

#include "sock.h"

/* * * * client * * * */
/**
 * \struct Client
 * \brief Client object
 */
typedef struct
{
  SOCKET sock; // socket infos
  char id[BUF_SIZE]; // client's id
}Client;

int      client__compare     (Client *a, Client *b);
void     client__set_id      (Client *c, const char *id);
char    *client__get_id      (Client *c);
char    *client__get_address (Client *c);
int      client__get_port    (Client *c);

#endif
