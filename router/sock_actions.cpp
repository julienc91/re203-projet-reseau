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

	char *tmp2 = mess__toString(m);
	network__send(c, tmp2); //segfault tant qu'on a pas un vrai c
	free(tmp2);
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
	char *tmp = mess__toString(m);
	network__send(c, tmp); //segfault tant qu'on a pas un vrai c
	free(tmp);
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
	char *tmp = mess__toString(m);
	network__send(router->getController(), tmp);
	mess__free(&m);
	free(tmp);
}


void SockActions::poll()
{
	Message* m;
	mess__init(&m);
	m->type = POLL;

	char *tmp = mess__toString(m);
	network__send(router->getController(), tmp);
	mess__free(&m);
	free(tmp);
}

void SockActions::logout()
{
	Message* m;
	mess__init(&m);
	m->type = LOGOUT;

	char *tmp = mess__toString(m);
	network__send(router->getController(), tmp);
	mess__free(&m);
	free(tmp);
}


void SockActions::link(Client *t)
{
	Message* m;
	mess__init(&m);
	m->type = LINK;

	m->node1 = strcopy(router->getName());

	char *tmp = mess__toString(m);
	network__send(t, tmp);
	mess__free(&m);
	free(tmp);
}

void SockActions::linkAck(Client* t)
{
	Message* m;
	mess__init(&m);
	m->type = LINK;
	m->accept = OK;

	char *tmp = mess__toString(m);
	network__send(t, tmp);
	mess__free(&m);
	free(tmp);
}

void SockActions::vector(char* id, char * vect)
{
	Message* m;
	mess__init(&m);
	m->type = VECTOR;

	m->s_parameter = strcopy(vect);
	//~ std::cout << "\n s_param :" << m->s_parameter << std::endl;

	char *tmp = mess__toString(m);
	//~ std::cout << "vector :" << tmp << std::endl;
	network__send(router->getRouteTable()[std::string(id)].client(), tmp);
	mess__free(&m);
	free(tmp);
}

void SockActions::vectorAck(char* id)
{
	Message* m;
	mess__init(&m);
	m->type = VECTOR;
	m->accept = OK;

	char *tmp = mess__toString(m);
	//~ std::cout << "vector ack:" << tmp << std::endl;
	network__send(router->getRouteTable()[std::string(id)].client(), tmp);
	mess__free(&m);
	free(tmp);
}
