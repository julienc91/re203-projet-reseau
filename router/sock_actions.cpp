#include "sock_actions.hpp"
#include "exceptions.hpp"

#include <cstring>
#include <iostream>


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


void SockActions::login(int port, char* id)
{
	Message* m;
	mess__init(&m);

	m->type = LOGIN;
	m->n_parameter = port;

	if(id != 0)
	{
		m->node1 = strcopy(id);
	}

	network__send(router->getController(), mess__toString(m));
	mess__free(&m);
}


void SockActions::poll()
{
	Message* m;
	mess__init(&m);
	m->type = POLL;

	network__send(router->getController(), mess__toString(m));
	mess__free(&m);
}

void SockActions::logout()
{
	Message* m;
	mess__init(&m);
	m->type = LOGOUT;

	network__send(router->getController(), mess__toString(m));
	mess__free(&m);
}


void SockActions::link(Client *t)
{
	Message* m;
	mess__init(&m);
	m->type = LINK;

	m->node1 = strcopy(router->getName());

	network__send(t, mess__toString(m));
	mess__free(&m);
}

void SockActions::linkAck(Client* t)
{
	Message* m;
	mess__init(&m);
	m->type = LINK;
	m->accept = OK;

	network__send(t, mess__toString(m));
	mess__free(&m);
}

void SockActions::vector(char* id, char * vect)
{
	Message* m;
	mess__init(&m);
	m->type = VECTOR;
	m->s_parameter = strcopy(vect);

	network__send(router->getRouteTable()[std::string(id)].client(), mess__toString(m));
}
