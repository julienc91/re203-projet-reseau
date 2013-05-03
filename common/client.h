#ifndef CLIENT_H_
#define CLIENT_H_

#include "sock.h"

/* * * * client * * * */
typedef struct
{
  SOCKET sock;
  char id[BUF_SIZE];
}Client;

int      client__compare     (Client *a, Client *b);
void     client__set_id      (Client *c, const char *id);
char    *client__get_id      (Client *c);
char    *client__get_address (Client *c);
int      client__get_port    (Client *c);

#endif
