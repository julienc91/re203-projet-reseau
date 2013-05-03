#include "client.h"

#include <string.h>
#include <stdio.h>

int client__compare (Client *a, Client *b){
  if (a == NULL && b == NULL) return 1;
  if (a == NULL || b == NULL) return 0;
  return ((a->sock == b->sock) && (strcmp(a->id, b->id) == 0));
}

void client__set_id(Client *c, const char *id){
  strcpy(c->id, id);
}

char *client__get_id(Client *c){
  return c->id;
}

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
