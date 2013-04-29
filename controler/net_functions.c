#include "../common/net.h"
#include "../common/messages.h"
#include "exec.h"
#include <stdio.h>

void input_event(network *net, char *string)
{
	printf("Pouet Input! \n");

	
}

void connection_event(network *net, Client *client, char *string)
{
	printf("Pouet ! \n");
	struct Message *message = mess__parse(mess__treatInput(string));
	message = exec__sock_message(message);
	client__set_id(client, message->node1);
}

void disconnection_event(network *net, Client *client)
{
		printf("Pouet disco! \n");

}

void message_event(network *net, Client *client, char *string)
{
		printf("Pouet mess! \n");

}
