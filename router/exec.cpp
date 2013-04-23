#include <iostream>
#include "exec.hpp"

Exec::Exec()
{
	net = network__open(12345); // why ?
}
static void Exec::prompt_message(Message* m)
{
	if(m == NULL)
	{
		return;
	}

	switch(m->type)
	{
		case MESSAGE:
			break;
		case PING:
			break;
		case ROUTE:
			break;
		case ROUTETABLE:
			break;
		default:
			cout << "ERREUR: Action non reconnue" << std::endl;
			break;
	}
}

static void Exec::sock_message(Message* m)
{
	if(m == NULL)
	{
		return;
	}

	switch(m->type)
	{
		case LINK:
			break;
		case VECTOR:
			break;
		case PACKET:
			break;
		case PING:
			break;
		case PONG:
			break;
		default:
			cout << "ERREUR: Action non reconnue" << std::endl;
			break;
	}
}
