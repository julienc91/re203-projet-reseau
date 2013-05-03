#include "sock_actions.hpp"

SockActions::SockActions(Router* r)
{
	router = r;
}

void SockActions::reverse(Message* m)
{
	char* tmp = m->node1;
	m->node1 = m->node2; //on inverse les pointeurs
	m->node2 = tmp;

	// Client* c = 0; // = ... celui qui correspond à mess->node1
	// network__send(c, mess__toString(packet)); //segfault tant qu'on a pas un vrai c
}

void SockActions::forward(Message* m)
{
	// Client* c = 0; // = ... celui qui correspond à mess->node1
	// network__send(c, mess__toString(packet)); //segfault tant qu'on a pas un vrai c
}
