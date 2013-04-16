#include "net.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>



/* private functions */


static int  init_server_connection(unsigned int port);
static int  init_client_connection(const char *address, const unsigned int port);
static int  read_client(SOCKET sock, char *buffer);
static void write_client(SOCKET sock, const char *buffer);
static void send_message_to_all_clients(Client *clients, Client client, int actual, const char *buffer, char from_server);
static void remove_client(Client *clients, int to_remove, int *actual);

/* * * * * * * * * * */


static void init(void)
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


static void end(void)
{
#ifdef WIN32
  WSACleanup();
#endif
}



network *new_network(unsigned int port){
  network *net = malloc(sizeof(*net));
  
  net->server = init_server_connection(port);
  net->nb_clients = 0;
  net->max = net->server;
  net->clients = malloc(sizeof(*(net->clients)) * NB_CLIENTS);
}

void free_network(network *net){
  closesocket(net->server);
  int i = 0;
  for(i = 0; i < net->nb_clients; i++){
    closesocket(net->clients[i].sock);
  }
  free(net->clients);
  free(net);
}

void update_network(network *net){
  char buffer[BUF_SIZE];

  fd_set rdfs;
  FD_ZERO(&rdfs);
  
  /* add STDIN_FILENO */
  FD_SET(STDIN_FILENO, &rdfs);
  
  /* add the connection socket */
  FD_SET(net->server, &rdfs);

  /* add socket of each client */
  int i;
  for(i = 0; i < net->nb_clients; i++)
    {
      FD_SET(net->clients[i].sock, &rdfs);
    }

  if(select(net->max + 1, &rdfs, NULL, NULL, NULL) == -1)
    {
      perror("select()");
      exit(errno);
    }

  /* something from standard input : i.e keyboard */
  if(FD_ISSET(STDIN_FILENO, &rdfs))
    {
      net->input_event(net);
      /* stop process when type on keyboard */
      return;
    }
  else if(FD_ISSET(net->server, &rdfs))
    {
      /* new client */
      SOCKADDR_IN csin = { 0 };
      size_t sinsize = sizeof csin;
      int csock = accept(net->server, (SOCKADDR *)&csin, (socklen_t *)&sinsize);
      if(csock == SOCKET_ERROR)
	{
	  perror("accept()");
	  return;
	}

      /* after connecting the client sends something */
      if(read_client(csock, buffer) == -1)
	{
	  /* disconnected */
	  return;
	}

      Client c;
      c.sock = csock;

      // TODO : traiter la connexion du client
      /* printf("New client connected : %s\n", buffer); */
      /* strncpy(c.name, buffer, BUF_SIZE - 1); */
      net->connection_event(net, &c, buffer);

      /* what is the new maximum fd ? */
      net->max = csock > net->max ? csock : net->max;

      FD_SET(csock, &rdfs);

      net->clients[net->nb_clients] = c;
      (net->nb_clients)++;
    }
  else
    {
      int i;
      for(i = 0; i < net->nb_clients; i++)
	{
	  /* a client is talking */
	  if(FD_ISSET(net->clients[i].sock, &rdfs))
            {
	      Client client = net->clients[i];
	      int c = read_client(client.sock, buffer);
	      /* client disconnected */
	      if(c == 0)
		{
		  /* gérer la déconnexion d'un client */
                  /* strncpy(buffer, client.name, BUF_SIZE - 1); */
                  /* strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1); */
                  //send_message_to_all_clients(clients, client, actual, buffer, 1);
		  net->deconnection_event(net, &client);

		  // close socket
                  closesocket(client.sock);
		  // update array
                  remove_client(net->clients, i, &(net->nb_clients));

		}
	      else
		{
		  /* gérer la réception du message */
		  //send_message_to_all_clients(clients, client, actual, buffer, 0);
		  net->message_event(net, &client, buffer);
		}
	      break;
            }
	}
    }
}




static void remove_client(Client *clients, int to_remove, int *actual)
{
  /* we remove the client in the array */
  memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
  /* number client - 1 */
  (*actual)--;
}

static void send_message_to_all_clients(Client *clients, Client sender, int actual, const char *buffer, char from_server)
{
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

static int init_server_connection(unsigned int port)
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
      exit(errno);
   }

   hostinfo = gethostbyname(address);
   if (hostinfo == NULL)
   {
      fprintf (stderr, "Unknown host %s.\n", address);
      exit(EXIT_FAILURE);
   }

   sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   sin.sin_port = htons(port);
   sin.sin_family = AF_INET;

   if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
   {
      perror("connect()");
      exit(errno);
   }

   return sock;
}

static int read_client(SOCKET sock, char *buffer)
{
  int n = 0;

  if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
    {
      perror("recv()");
      /* if recv error we disonnect the client */
      n = 0;
    }

  buffer[n] = 0;

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



/* * * * * * * * * * * TESTS * * * * * * * * * * */

int main(int argc, char **argv)
{
  init();

  network *net = new_network(25565);

  while(1){
    update_network(net);
    //    connect_network(address, port);
  }

  free_network(net);


  end();

  return EXIT_SUCCESS;
}
