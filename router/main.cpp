extern "C" {
#include "../common/net.h"
#include "../common/messages.h"
}
#include "event.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>


int main(int argc, char **argv)
{
	if (argc != 4)
	{
		fprintf(stderr, "Usage: %s <port serveur> <port connexion> <id>\n", argv[0]);
		return EXIT_FAILURE;
	}
	unsigned int port = atoi(argv[2]);
	Event evhandler;
	init(); // windows compatibility

	// * * * * ouverture serveur * * * *
	network *net = network__open(atoi(argv[1]));

	// * * * * evenements * * * *
	net->input_event =  evhandler.input;
	net->connection_event = evhandler.connect;
	net->disconnection_event = evhandler.disconnect;
	net->message_event = evhandler.message;

	// * * * * connexion sortante * * * *
	printf("Connection to localhost on port %d...\n",port);
	Client *c = network__connect(net, "localhost", port);

	if (!c)
	{
		fprintf(stderr, "Connection failed.\n");
	}
	else
	{
		strcpy(c->id, "controlleur");
		printf("Connected on socket %d.\n", c->sock);
		char buf[100];
		sprintf(buf, "log in as %s", argv[3]);
		network__send(c, buf);
	}

	// * * * * gestion serveur * * * *
	while(network__is_opened(net))
	{
		network__update(net);
	}

	// * * * * fermeture serveur * * * *
	network__close(net);
	network__free(net);


	end(); // windows compatibility

	return EXIT_SUCCESS;
}
