#include "sock_actions.hpp"
#include "exceptions.hpp"
SockActions::SockActions(Router* r)
{
	router = r;
}

void SockActions::reverse(Message* m)
{
	char* tmp = m->node1;
	m->node1 = m->node2; //on inverse les pointeurs
	m->node2 = tmp;


	// on regarde le next hop dans la table de routage
	Client* c = 0;

	try
	{
		c = router->getRouteTable().nextClient(std::string(m->node2));
	}
	catch(UnknownDest)
	{
		throw;
	}

	network__send(c, mess__toString(m)); //segfault tant qu'on a pas un vrai c
}

void SockActions::forward(Message* m)
{
	// on regarde le next hop dans la table de routage
	Client* c = 0;

	try
	{
		c = router->getRouteTable().nextClient(std::string(m->node2));
	}
	catch(UnknownDest)
	{
		throw;
	}

	network__send(c, mess__toString(m)); //segfault tant qu'on a pas un vrai c
}
