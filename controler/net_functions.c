#include "../common/net.h"
#include "../common/messages.h"
#include "exec.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void input_event(network *net, char *string)
{
	printf("Pouet Input! \n");
}

void connection_event(network *net, Client *client, char *string)
{
	printf("Pouet ! \n");
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
	message->type = GREETING;
	network__send(client, mess__treatOutput(mess__toString(message)));
}

void disconnection_event(network *net, Client *client)
{
		printf("Pouet disco! \n");

}

void message_event(network *net, Client *client, char *string)
{
		printf("Pouet mess! \n");

}
