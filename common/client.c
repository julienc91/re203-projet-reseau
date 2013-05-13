/**
 * \file client.c
 * \brief Client structure and useful functions
 * 
 * Data structure representing a client for communication
 * within the network.
 * 
 */

#include "client.h"

#include <string.h>
#include <stdio.h>

/**
 *  \brief Compare two clients
 *  \param a A client.
 *  \param b Another client.
 *  \return 1 if a and b are representing the exact same client, 0 otherwise.
 */
int client__compare (Client *a, Client *b){
  if (a == NULL && b == NULL) return 1;
  if (a == NULL || b == NULL) return 0;
  return ((a->sock == b->sock) && (strcmp(a->id, b->id) == 0));
}

/**
 *  \brief Change a client's id
 *  \param c A client.
 *  \param id The new id to use.
 */
void client__set_id(Client *c, const char *id){
  strcpy(c->id, id);
}

/**
 *  \brief Get a client's id
 *  \param c A client.
 *  \return The given client's id.
 */
char *client__get_id(Client *c){
  return c->id;
}

/**
 *  \brief Get a client's address
 *  \param c A client.
 *  \return The client's ip address in the IPV4 notation.
 */
char *client__get_address(Client *c) {
  struct sockaddr_in sin;
  socklen_t len = sizeof(sin);
  if (getsockname(c->sock, (struct sockaddr *)&sin, &len) == -1){
    perror("getsockname");
  }
  else{
    return inet_ntoa(sin.sin_addr);
  }
}

/**
 *  \brief Get a client's port
 *  \param c A client.
 *  \return The client's port.
 */
int client__get_port(Client *c) {
  struct sockaddr_in sin;
  socklen_t len = sizeof(sin);
  if (getsockname(c->sock, (struct sockaddr *)&sin, &len) == -1){
    perror("getsockname");
  }
  else{
    return ntohs(sin.sin_port);
  }
}
