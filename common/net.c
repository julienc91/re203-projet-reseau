#include "../common/net.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define NETWORK__OPENED 1
#define NETWORK__CLOSED 0

/* private functions */


static int     init_server_connection (unsigned int port);
static int     init_client_connection (const char *address, const unsigned int port);
static int     read_client            (SOCKET sock, char *buffer);
static void    write_client           (SOCKET sock, const char *buffer);
static Client *add_client             (network *net, Client *c);
static int     get_client_id          (network *net, Client *c);
static void    remove_client          (network *net, Client *c);
void           broadcast              (Client *clients, Client client,int actual,
				       const char *buffer, char from_server);

/* * * * * * * * * * */

void init(void)
{
#ifdef WIN32
  WSADATA wsa;
  int err = WSAStartup(MAKEWORD(2, 2), &wsa);
  if(err < 0)
    {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
    }
#endif
}


void end(void)
{
#ifdef WIN32
  WSACleanup();
#endif
}



network *network__open(unsigned int port){
  network *net = malloc(sizeof(*net));

  net->status = NETWORK__OPENED;
  net->server = init_server_connection(port);
  net->nb_clients = 0;
  net->max = net->server;
  net->clients = malloc(sizeof(*(net->clients)) * NB_CLIENTS);

  return net;
}

void network__close(network *net){
  closesocket(net->server);
  unsigned int i = 0;
  for(i = 0; i < net->nb_clients; i++){
    closesocket(net->clients[i].sock);
  }

  net->status = NETWORK__CLOSED;
}

int network__is_opened (network *net){
  return (net->status == NETWORK__OPENED);
}

void network__free(network *net){
  if (network__is_opened(net)) network__close(net);
  free(net->clients);
  free(net);
}

Client *network__connect(network *net, const char *address, const unsigned int port){
  Client c;
  c.sock = init_client_connection(address, port);

  if (c.sock == CONNECTION_ERROR) return NULL;
  return add_client(net, &c);
}

void network__disconnect (network *net, Client *c){
  // close socket
  closesocket(c->sock);
  // update array
  remove_client(net, c);
}

void network__send(Client *c, const char *message){
  write_client (c->sock, message);
}

void network__broadcast  (network *net, const char *message){
  unsigned int i;
  for (i = 0; i < net->nb_clients; i++){
    network__send(&(net->clients[i]), message);
  }
}



void network__update(network *net){
  if (net == NULL) return;
  if (!network__is_opened(net)) return;

  char buffer[BUF_SIZE];

  fd_set rdfs;
  FD_ZERO(&rdfs);

  /* add STDIN_FILENO */
  FD_SET(STDIN_FILENO, &rdfs);

  /* add the connection socket */
  FD_SET(net->server, &rdfs);

  /* add socket of each client */
  unsigned int i;
  for(i = 0; i < net->nb_clients; i++){
    FD_SET(net->clients[i].sock, &rdfs);
  }

  if(select(net->max + 1, &rdfs, NULL, NULL, NULL) == -1){
    perror("select()");
    exit(errno);
  }

  /* something from standard input : i.e keyboard */
  //if(FD_ISSET(STDIN_FILENO, &rdfs)){
    //int c;
    //char *buf = buffer;
    //while ((c = getchar()) != '\n' && c != EOF){*(buf++) = c;}
    //*buf = '\0';
    //net->input_event(net, buffer);
    //fflush(stdin); // force flush
//
    /* stop process when type on keyboard */
    //return;
  //}
  //else
  //
  if(FD_ISSET(net->server, &rdfs)){
    /* new client */
    SOCKADDR_IN csin = { 0 };
    size_t sinsize = sizeof csin;
    int csock = accept(net->server, (SOCKADDR *)&csin, (socklen_t *) &sinsize);
    if(csock == SOCKET_ERROR){
      perror("accept()");
      return;
    }

    /* after connecting the client sends something */
    if(read_client(csock, buffer) == -1){
      /* disconnected */
      return;
    }

    Client c;
    c.sock = csock;

    // client connection
    net->connection_event(net, &c, buffer);

    add_client(net, &c);
    FD_SET(csock, &rdfs);
  }
  else{
    unsigned int i;
    for(i = 0; i < net->nb_clients; i++){
      /* a client is talking */
      if(FD_ISSET(net->clients[i].sock, &rdfs)){
	Client client = net->clients[i];
	int c = read_client(client.sock, buffer);
	/* client disconnected */
	if(c == 0){
	  net->disconnection_event(net, &client);
	  network__disconnect(net, &client);
	}
	else{
	  /* message received */
	  net->message_event(net, &client, buffer);
	}
	break;
      }
    }
  }
}

static int get_client_id (network *net, Client *c){
  unsigned int i;
  for (i = 0; i < net->nb_clients; i++){
    if (client__compare(c, &(net->clients[i]))) return i;
  }
  return -1;
}

static Client *add_client(network *net, Client *c){
  /* what is the new maximum fd ? */
  net->max = c->sock > net->max ? c->sock : net->max;

  net->clients[net->nb_clients] = *c;
  (net->nb_clients)++;

  return &(net->clients[net->nb_clients - 1]);
}

static void remove_client(network *net, Client *c){
  // get client id
  int to_remove = get_client_id(net, c);
  if (to_remove < 0){
    fprintf(stderr, "Error: remove_client\n");
    return;
  }

  // free memory
  memmove(net->clients + to_remove, net->clients + to_remove + 1, (net->nb_clients - to_remove - 1) * sizeof(Client));

  // change nb of clients
  (net->nb_clients)--;
}

void broadcast(Client *clients, Client sender, int actual, const char *buffer, char from_server){
  int i = 0;
  char message[BUF_SIZE];
  message[0] = 0;
  for(i = 0; i < actual; i++)
    {
      /* we don't send message to the sender */
      if(sender.sock != clients[i].sock)
	{
	  if(from_server == 0)
	    {
	      strncpy(message, sender.id, BUF_SIZE - 1);
	      strncat(message, " : ", sizeof message - strlen(message) - 1);
	    }
	  strncat(message, buffer, sizeof message - strlen(message) - 1);
	  write_client(clients[i].sock, message);
	}
    }
}

int init_server_connection(unsigned int port)
{
  SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
  SOCKADDR_IN sin = { 0 };

  if(sock == INVALID_SOCKET)
    {
      perror("socket()");
      exit(errno);
    }

  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(port);
  sin.sin_family = AF_INET;

  if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
    {
      perror("bind()");
      exit(errno);
    }

  if(listen(sock, NB_CLIENTS) == SOCKET_ERROR)
    {
      perror("listen()");
      exit(errno);
    }

  return sock;
}

static int init_client_connection(const char *address, const unsigned int port)
{
  SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
  SOCKADDR_IN sin = { 0 };
  struct hostent *hostinfo;

  if(sock == INVALID_SOCKET)
    {
      perror("socket()");
      //exit (errno);
      return CONNECTION_ERROR;
    }

  hostinfo = gethostbyname(address);
  if (hostinfo == NULL)
    {
      fprintf (stderr, "Unknown host %s.\n", address);
      //exit(EXIT_FAILURE);
      return CONNECTION_ERROR;
    }

  sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
  sin.sin_port = htons(port);
  sin.sin_family = AF_INET;

  if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
      perror("connect()");
      //exit(errno);
      return CONNECTION_ERROR;
    }

  return sock;
}

static int read_client(SOCKET sock, char *buffer)
{
  int n = 0;

  if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
    {
      perror("recv()");
      /* if recv error we disconnect the client in the update function */
      n = 0;
    }

  buffer[n] = '\0';

  return n;
}

static void write_client(SOCKET sock, const char *buffer)
{
  if(send(sock, buffer, strlen(buffer), 0) < 0)
    {
      perror("send()");
      exit(errno);
    }
}

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
