#include "prompt_actions.hpp"
#include "exceptions.hpp"
#include <cstring>

#include <iostream>
extern "C"
{
	#include "../common/util.h"
}

PromptActions::PromptActions(Router* r)
{
	router = r;
}


void PromptActions::message(Message* mess)
{
	struct Message* packet;
	mess__init(&packet);

	packet->seqnum = router->newSeqnum(); // devra faire l'incrémentation'
	packet->node1 = strcopy(router->getName()); //source
	packet->node2 = strcopy(mess->node1); // destination
	packet->n_parameter = router->getConfiguration()->defaultTTLValue;
	packet->s_parameter = strcopy(mess->s_parameter);


	// on regarde le next hop dans la table de routage
	Client* c = 0;

	try
	{
		c = router->getRouteTable().nextClient(std::string(packet->node2));
	}
	catch(UnknownDest)
	{
		throw;
	}


	network__send(c, mess__toString(packet));
}


void PromptActions::ping(Message* mess)
{
	struct Message* ll_ping; // ping de bas niveau (low level)
	mess__init(&ll_ping);

	ll_ping->seqnum = router->newSeqnum();
	ll_ping->node1 = strcopy(router->getName());
	ll_ping->node2 = strcopy(mess->node1);
	ll_ping->n_parameter = router->getConfiguration()->defaultTTLValue;

	// on regarde le next hop dans la table de routage
	Client* c = 0;

	try
	{
		c = router->getRouteTable().nextClient(std::string(ll_ping->node2));
	}
	catch(UnknownDest)
	{
		throw;
	}

	for (int i = 0; i < router->getConfiguration()->defaultPingPacketCount; i++)
	{
		network__send(c, mess__toString(ll_ping)); //segfault tant qu'on a pas un vrai c
	}
}


void PromptActions::route(Message* mess)
{
	struct Message* ll_ping; // ping de bas niveau (low level)
	mess__init(&ll_ping);

	ll_ping->seqnum = router->newSeqnum();
	ll_ping->node1 = strcopy(router->getName());
	ll_ping->node2 = strcopy(mess->node1);

	// on regarde le next hop dans la table de routage
	Client* c = 0;

	try
	{
		c = router->getRouteTable().nextClient(std::string(ll_ping->node2));
	}
	catch(UnknownDest)
	{
		throw;
	}

	int i = 1;
	while(i < 100) // max ttl ? temps ? tant qu'on n'a pas reçu de réponse favorable à notre route ?
	//peut être exécuter dans un thread avec incrémentation et comparaison d'une variable dans la classe (mutex)
	{
		ll_ping->n_parameter = i;
		network__send(c, mess__toString(ll_ping));
	}
}
