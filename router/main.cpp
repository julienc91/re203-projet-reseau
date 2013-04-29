extern "C" {
#include "../common/net.h"
#include "../common/messages.h"
#include "../common/prompt.h"
#include "../common/config.h"
}
#include "event.hpp"
#include "exec.hpp"
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
	init(); // windows compatibility

	// * * * * lecture fichier config * * * *
	Configuration* routeur_conf = config__readRouter();

	// * * * * ouverture serveur * * * *
	network *net = network__open(atoi(argv[1]));

	// * * * * evenements * * * *
	net->input_event =  Event::input;
	net->connection_event = Event::connect;
	net->disconnection_event = Event::disconnect;
	net->message_event = Event::message;

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

	// * * * * gestion stdin   * * * *
	pthread_t *prompt_th1 = prompt__start(Exec::prompt_message);

	// * * * * gestion serveur * * * *
	while(network__is_opened(net))
	{
		network__update(net);
	}

	// * * * * fermeture serveur * * * *
	pthread_join(*prompt_th1, NULL);
	network__close(net);
	network__free(net);


	end(); // windows compatibility

	return EXIT_SUCCESS;
}
