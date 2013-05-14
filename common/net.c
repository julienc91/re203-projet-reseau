/**
 * \file net.c
 * \brief Network manager
 *
 * Opens a server socket and manages connections
 * as an array of "Client"s.
 *
 */

#include "../common/net.h"
#include "../common/messages.h"
#include "../common/util.h"
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
static int     write_client           (SOCKET sock, const char *buffer);
static Client *add_client             (network *net, Client *c);
static int     get_client_id          (network *net, Client *c);
static void    remove_client          (network *net, Client *c);
void           broadcast              (Client *clients, Client client,int actual,
				       const char *buffer, char from_server);

/* * * * * * * * * * */

/**
 *  \fn void init(void)
 *  \brief Enables windows compatibility
 *  Should be used before any functions.
 */
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

/**
 *  \fn void init(void)
 *  \brief Enables windows compatibility
 *  Should be used after any functions.
 */
void end(void)
{
#ifdef WIN32
  WSACleanup();
#endif
}

/**
 * \fn network *network__open(unsigned int port)
 * \brief Initializes the network and open the listening socket
 *
 * \param port Port used by the server socket.
 * \return A network object.
 *
 * Set the network status to opened.
 */
network *network__open(unsigned int port){
  network *net = malloc(sizeof(*net));

  net->status = NETWORK__OPENED;
  net->server = init_server_connection(port);
  net->nb_clients = 0;
  net->max = net->server;
  net->clients = malloc(sizeof(*(net->clients)) * NB_CLIENTS);

  return net;
}

/**
 * \fn void network__close(network *net)
 * \brief Close every connections and the server socket.
 * \param net A network object.
 *
 * Set the network status to closed.
 */
void network__close(network *net){
  closesocket(net->server);
  unsigned int i = 0;
  for(i = 0; i < net->nb_clients; i++){
    closesocket(net->clients[i].sock);
  }

  net->status = NETWORK__CLOSED;
}

/**
 * \fn int network__is_opened (network *net)
 * \brief Checks if the network is opened.
 * \param net A network object.
 * \return Boolean, true if opened.
 */
int network__is_opened (network *net){
  return (net->status == NETWORK__OPENED);
}

/**
 * \fn void network__free(network *net)
 * \brief Destroy a network object
 * \param net A network object.
 * Will close every connections remaining if the network is still opened.
 */
void network__free(network *net){
  if (network__is_opened(net)) network__close(net);
  free(net->clients);
  free(net);
}

/**
 * \fn Client *network__connect(network *net, const char *address, const unsigned int port)
 * \brief Outgoing connection to a new client.
 * \param net A network object.
 * \param address IP address of the client.
 * \param port Port that the client is listening to.
 * \return A Client object, which can be used to specify its "id", if the connexion was successful, NULL otherwise.
 *
 * The new Client is added to the network array.
 */
Client *network__connect(network *net, const char *address, const unsigned int port){
  Client c;
  c.sock = init_client_connection(address, port);

  if (c.sock == NETWORK__CONNECTION_ERROR) return NULL;
  return add_client(net, &c);
}

/**
 * \fn void network__disconnect (network *net, Client *c)
 * \brief Disconnect a Client.
 * \param net A network object.
 * \param c A client object.
 * The local socket is closed before the client's one.
 */
void network__disconnect (network *net, Client *c){
  // close socket
  closesocket(c->sock);
  // update array
  remove_client(net, c);
}

/**
 * \fn int network__send(Client *c, const char *message)
 * \brief Sends a message to a Client.
 * \param c A client object
 * \param message The string to send.
 * \return Bytes successfuly sent, NETWORK__SEND_ERROR (< 0) otherwise.
 */
int network__send(Client *c, const char *message)
{
	#ifdef __ROUTER_DEBUG
  fprintf(stderr, "<Sending message to '%s' on socket '%d': '%s'>\n", c->id, (int)c->sock, message);
  #endif
  char* tmp = strcopy(message);
  char* mess2 = mess__treatOutput(tmp);
  if(!mess2)
       return NETWORK__SEND_ERROR;
  int r = write_client (c->sock, mess2);
  free(mess2);
  free(tmp);
  return r;
}

/**
 * \fn int network__broadcast  (network *net, const char *message)
 * \brief Sends a message to every clients connected.
 * \param net A network object
 * \param message The string to send.
 * \return Bytes successfuly sent, NETWORK__SEND_ERROR (< 0) otherwise.
 */
int network__broadcast  (network *net, const char *message){
  int broadcast_return_value = 0;
  int return_value = 0, failed = 0;
  unsigned int i;
  for (i = 0; i < net->nb_clients; i++){
    return_value = network__send(&(net->clients[i]), message);
    if (return_value < 0) failed = 1;
    broadcast_return_value += return_value;
  }
  return failed ? NETWORK__SEND_ERROR : broadcast_return_value;
}


/**
 * \fn void network__update(network *net)
 * \brief Updates the network and raises events.
 * \param net A network object
 * Should be used in the main loop of the program
 * (continue while the server is opened).
 * This function will raise events (function pointers)
 * which must be set in the network object.
 */
void network__update(network *net){
  if (net == NULL) return;
  if (!network__is_opened(net)) return;

  char buffer[BUF_SIZE];

  fd_set rdfs;
  FD_ZERO(&rdfs);

  /* add STDIN_FILENO */
  //FD_SET(STDIN_FILENO, &rdfs);

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
  if(FD_ISSET(STDIN_FILENO, &rdfs)){
    int c;
    char *buf = buffer;
    while ((c = getchar()) != '\n' && c != EOF){*(buf++) = c;}
    *buf = '\0';
    net->input_event(net, buffer);
    fflush(stdin); // force flush

    /* stop process when type on keyboard */
    return;
  }
  else if(FD_ISSET(net->server, &rdfs)){
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

/**
 * \fn static int get_client_id (network *net, Client *c)
 * \brief Index of a Client in the network array.
 * \param net A network object.
 * \param c A client object.
 * \return The index if the client if found, -1 otherwise.
 */
static int get_client_id (network *net, Client *c){
  unsigned int i;
  for (i = 0; i < net->nb_clients; i++){
    if (client__compare(c, &(net->clients[i]))) return i;
  }
  return -1;
}

/**
 * \fn static Client *add_client(network *net, Client *c)
 * \brief Add (by copy) a client the network array.
 * \param net A network object.
 * \param c A client object.
 * \return A reference to the client from the array.
 */
static Client *add_client(network *net, Client *c){
  /* what is the new maximum fd ? */
  net->max = c->sock > net->max ? c->sock : net->max;

  net->clients[net->nb_clients] = *c;
  (net->nb_clients)++;

  return &(net->clients[net->nb_clients - 1]);
}

/**
 * \fn static void remove_client(network *net, Client *c)
 * \brief Remove (and destroy) a client from the network array.
 */
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

/**
 * \fn void broadcast(Client *clients, Client sender, int actual, const char *buffer, char from_server)
 * \brief Sends a message to every clients.
 */
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

/**
 * \fn int init_server_connection(unsigned int port)
 * \brief Open a server socket (in listening mode).
 */
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

  /* trick to prevent 'bind():  address already in use' errors */
  int one = 1; setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

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

/**
 * \fn static int init_client_connection(const char *address, const unsigned int port)
 * \brief Open an outgoing client socket (in connect mode).
 */
static int init_client_connection(const char *address, const unsigned int port)
{
  SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
  SOCKADDR_IN sin = { 0 };
  struct hostent *hostinfo;

  if(sock == INVALID_SOCKET)
    {
      perror("socket()");
      //exit (errno);
      return NETWORK__CONNECTION_ERROR;
    }

  hostinfo = gethostbyname(address);
  if (hostinfo == NULL)
    {
      fprintf (stderr, "Unknown host %s.\n", address);
      //exit(EXIT_FAILURE);
      return NETWORK__CONNECTION_ERROR;
    }

  sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
  sin.sin_port = htons(port);
  sin.sin_family = AF_INET;

  if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
      perror("connect()");
      //exit(errno);
      return NETWORK__CONNECTION_ERROR;
    }

  return sock;
}

/**
 * \fn static int read_client(SOCKET sock, char *buffer)
 * \brief Reads a socket.
 */
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

/**
 * \fn static int write_client(SOCKET sock, char *buffer)
 * \brief Writes a socket.
 * \return 0 if successful, NETWORK__SEND_ERROR otherwise.
 */
static int write_client(SOCKET sock, const char *buffer)
{
    if(send(sock, buffer, strlen(buffer), 0) < 0)
    {
        perror("send()");
        return NETWORK__SEND_ERROR;
    }
    return 0;
}

/**
 * \fn void network__debug(network *net)
 * \brief Display a debug message describing the network state.
 * \param net A network object.
 */
void network__debug(network *net){
    fprintf(stderr, "[DEBUG] Network status : ");
    if (net->status==NETWORK__CLOSED){
        fprintf(stderr, "'closed'\n");
    } else {
        fprintf(stderr, "'opened'\n");
    }
    fprintf(stderr, "[DEBUG] Server socket : '%d'\n", (int)net->server);
    fprintf(stderr, "[DEBUG] %d client(s) connected :\n", net->nb_clients);
    int i;
    for (i = 0; i < net->nb_clients; i++){
        fprintf(stderr, "[DEBUG] (%d) id='%s', socket='%d'\n", i, net->clients[i].id, (int)(net->clients[i].sock));
    }
}
