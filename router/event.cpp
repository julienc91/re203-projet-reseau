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

	strcpy(c->id, buffer + 10);

	struct Message *m = mess__parse(mess__treatInput(buffer));

	if (!m) return;
	glob__router->exec->sock_message(m);

}

void Event::disconnect(network *net, Client *c)
{
	std::cout << "<disconnection from socket '" << c->sock << "' of client '"<< c->id <<"'>" << std::endl;
}

void Event::message(network *net, Client *c, char *buffer)
{
	std::cout << "<message from '" << c->id << "', socket " << c->sock <<" : '" << buffer << "'>" << std::endl;

	struct Message *m = mess__parse(mess__treatInput(buffer));

	if (!m) return;
	if(m->type == VECTOR)
	{
		m->node1 = strcopy(c->id);
	}
	glob__router->exec->sock_message(m);
}
