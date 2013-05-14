#include "../common/net.h"
#include "../common/client.h"
#include "../common/messages.h"
#include "../common/util.h"
#include "exec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void input_event(network *net, char *string)
{
	printf("Evènement clavier \n");//Obsolete
}

void connection_event(network *net, Client *client, char *string)
{
	printf("<connection on socket '%d' : '%s'>\n", (int)client->sock, string);

	//Necessaire pour les tests via telnet
	int i = strlen(string);
	while (i>=0 && string[i] != '*')
	{
		i--;
	}
	string[i+1] = '\0';

	Messages *m = mess__multiline_parse(string);

	if (m == NULL) return;

	unsigned int k;
	for (k = 0; k < m->nb_messages; k++)
	{
		Message *message = m->messages[k];
			if (message == NULL) continue;

		if (message->type == NONE)
		{
			network__disconnect(net, client);
			printf("Message de type NONE\n");
			mess__free_messages(&m);
			return ;
		}
		strcopy2(&message->s_parameter, client__get_address(client));
		message = exec__sock_message(message);
		if (message == NULL) {
			fprintf(stderr, "[CONTROLLER] Connexion failed.\n");
			network__send(client, "log out"); // proper disconnection (?)
			mess__free_messages(&m);
			return;
		}
		client__set_id(client, message->node1);

		network__send(client, mess__toString(message));
	}

	mess__free_messages(&m);
}

void disconnection_event(network *net, Client *client)
{
	printf("<disconnection from socket '%d' of client '%s'>\n", (int)client->sock, client->id);
    char *tmp = mess__treatInput("log out*");
	struct Message *message = mess__parse(tmp);
	strcopy2(&message->node1, client__get_id(client));
	exec__sock_message(message);
	
    free(tmp);
	mess__free(&message);
}

void message_event(network *net, Client *client, char *string)
{
    if (client != NULL && client->id != NULL)
		printf("<message from '%s', socket '%d' : '%s'>\n", client->id, (int)client->sock, string);

		//Necessaire pour les tests via telnet
		int i = strlen(string);
		while (i>=0 && string[i] != '*')
		{
			i--;
		}
		string[i+1] = '\0';

		Messages *m = mess__multiline_parse(string);
		if (m == NULL) return;

		unsigned int k;
		for (k = 0; k < m->nb_messages; k++)
		{
				Message *message = m->messages[k];
					if (message == NULL) continue;

				if (message->type == NONE || message->type == VECTOR || message->type == PACKET || message->type == PING || message->type == PONG)
				{
					fprintf(stderr, "Message invalide pour le contrôleur\n");
					continue ;
				}
				strcopy2(&message->node1, client__get_id(client));
				message = exec__sock_message(message);
				if(!message)
				     return;

				network__send(client, mess__toString(message));
				if(message->type == BYE)
				{
					network__disconnect(net, client);
					mess__free_messages(&m);
					return;
				}
		}

		mess__free_messages(&m);
		//~ free(string);
}
