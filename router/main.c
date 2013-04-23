#include "../common/net.h"
//#include "../common/prompt.h"
#include "../common/messages.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void input_event (network *net, char *buffer){ 
  printf("<input event: '%s'>\n", buffer);
  
  if (strcmp(buffer, "quit") == 0){
    network__close(net);
    return;
  }

  network__send(&(net->clients[0]), buffer);
}

void connection_event (network *net, Client *c, char *buffer){ 
  printf("<connection on socket %d : %s>\n", c->sock, buffer);
  strcpy(c->id, buffer + 10);
}

void disconnection_event (network *net, Client *c){ 
  printf("<disconnection from socket %d>\n", c->sock);
}

void message_event (network *net, Client *c, char *buffer){ 
  printf("<message from '%s', socket %d : %s>\n", c->id, c->sock, buffer);

  struct Message *m = mess__parse(buffer);
  
  if (!m) return;
  switch(m->type){
  case LOGIN:
   
  default:
    fprintf(stderr, "Error: action unknown");
    break;
  }
}

//~ mess__free(m);

int main(int argc, char **argv){
  if (argc != 4){
    fprintf(stderr, "Usage: %s <port serveur> <port connexion> <id>\n", argv[0]);
    return EXIT_FAILURE;
  }
  unsigned int port = atoi(argv[2]);
 
  init(); // windows compatibility

  // * * * * ouverture serveur * * * *
  network *net = network__open(atoi(argv[1]));

  // * * * * evenements * * * *
  net->input_event = input_event;
  net->connection_event = connection_event;
  net->disconnection_event = disconnection_event;
  net->message_event = message_event;

  // * * * * connexion sortante * * * *
  printf("Connection to localhost on port %d...\n",port);
  Client *c = network__connect(net, "localhost", port);

  if (!c){
    fprintf(stderr, "Connection failed.\n");
  }else{
    strcpy(c->id, "controlleur");
    printf("Connected on socket %d.\n", c->sock);
    char buf[100];
    sprintf(buf, "log in as %s", argv[3]);
    network__send(c, buf);
  }

  // * * * * gestion serveur * * * *
  while(network__is_opened(net)){
    network__update(net);
  }

  // * * * * fermeture serveur * * * *
  network__close(net);
  network__free(net);


  end(); // windows compatibility

  return EXIT_SUCCESS;
}
