#include <iostream>
#include <cstring>

extern "C"
{
	#include "../common/net.h"
	#include "../common/messages.h"
}
#include "event.hpp"
#include "exec.hpp"

extern Router* glob__router;

void Event::input(network *net, char *buffer)
{
	std::cout << "<input event: '" << buffer << "'>" << std::endl;

	if (strcmp(buffer, "quit") == 0)
	{
		network__close(net);
		return;
	}

	network__send(&(net->clients[0]), buffer);
}

void Event::connect(network *net, Client *c, char *buffer)
{
	std::cout << "<connection on socket '" << c->sock << "' : '" << buffer << "'>" << std::endl;

	Messages *m = mess__multiline_parse(buffer);

	if (!m) return;

	for (unsigned int i = 0; i < m->nb_messages; i++)
	{
		if (m->messages[i] != NULL)
			glob__router->exec->sock_message(m->messages[i], c);
	}
	mess__free_messages(&m);
}

void Event::disconnect(network *net, Client *c)
{
	std::cout << "<disconnection from socket '" << c->sock << "' of client '"<< c->id <<"'>" << std::endl;
}

void Event::message(network *net, Client *c, char *buffer)
{
	std::cout << "<message from '" << c->id << "', socket " << c->sock <<" : '" << buffer << "'>" << std::endl;

	Messages *m = mess__multiline_parse(buffer);

	if (!m) return;

	for (unsigned int i = 0; i < m->nb_messages; i++)
	{
		if (m->messages[i] != NULL)
		{
			glob__router->exec->sock_message(m->messages[i], c);
		}
	}
	mess__free_messages(&m);
}
