#include "../common/net.h"
#include "../common/client.h"
#include "../common/messages.h"
#include "../common/util.h"
#include "exec.h"
#include <stdio.h>
#include <string.h>

void input_event(network *net, char *string)
{
	printf("Evènement clavier \n");//Obsolete
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

	if (message->type == NONE)
	{
		network__disconnect(net, client);
		printf("Message de type NONE\n");
		return ;
	}
	strcopy2(&message->s_parameter, client__get_address(client));	
	message = exec__sock_message(message);
	client__set_id(client, message->node1);
	
	network__send(client, mess__treatOutput(mess__toString(message)));
}

void disconnection_event(network *net, Client *client)
{
	printf("Deconnexion \n");
	struct Message *message = mess__parse(mess__treatInput("log out*"));
	strcopy2(&message->node1, client__get_id(client)); 
	exec__sock_message(message);
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

		if (message->type == NONE)
		{
			printf("Message de type NONE\n");
			return ;
		}
		strcopy2(&message->node1, client__get_id(client)); 
		message = exec__sock_message(message);

		network__send(client, mess__treatOutput(mess__toString(message)));
		if(message->type == BYE)
		{
			network__disconnect(net, client);
		}
}
