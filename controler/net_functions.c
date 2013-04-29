#include "../common/net.h"
#include "../common/messages.h"
#include "../common/util.h"
#include "exec.h"
#include <stdio.h>
#include <string.h>

void input_event(network *net, char *string)
{
	printf("Pouet Input! \n");
}

void connection_event(network *net, Client *client, char *string)
{
	//Necessaire pour les tests via telnet
	int i = strlen(string);
	while (i>=0 && string[i] != '*')
	{
		i--;
	}
	string[i+1] = '\0';

	struct Message *message = mess__parse(mess__treatInput(string));
	message = exec__sock_message(message);

	client__set_id(client, message->node1);
	network__send(client, mess__treatOutput(mess__toString(message)));
}

void disconnection_event(network *net, Client *client)
{
		printf("Pouet disco! \n");
		//~ struct Message *message;
		//~ mess__init(&message);
		//~ message->type = LOGOUT;
		//~ strcopy(message->node1, client__get_id(client)); 
		//~ message = exec__sock_message(message);

		//~ client__set_id(client, message->node1);
		//~ network__send(client, mess__treatOutput(mess__toString(message)));
}

void message_event(network *net, Client *client, char *string)
{
		//Necessaire pour les tests via telnet
		int i = strlen(string);
		while (i>=0 && string[i] != '*')
		{
			i--;
		}
		string[i+1] = '\0';

		struct Message *message = mess__parse(mess__treatInput(string));
		printf("Pouet disco! \n");

		if (message->type == NONE)
		{
			printf("Pouet disco2! \n");
			return ;
		}
		printf("NODE1 : %s\n", message->node1);
		printf("client : %s\n", client__get_id(client));
		strcopy2(message->node1, client__get_id(client)); 
		printf("Pouet disco3! \n");

		message = exec__sock_message(message);
		//~ client__set_id(client, message->node1);
		//~ message->type = GREETING;
		printf("Pouet disco4! \n");

		network__send(client, mess__treatOutput(mess__toString(message)));

}
