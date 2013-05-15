#include "prompt_actions.hpp"
#include "exceptions.hpp"
#include <cstring>

#include <unistd.h>
#include <iostream>
extern "C"
{
	#include "../common/util.h"
}

PromptActions::PromptActions(Router* r)
{
	router = r;
}


void PromptActions::message(Message* mess, std::map<int, int>& seqnumTable)
{
	struct Message* packet;
	mess__init(&packet);

	packet->type = PACKET;
	int seq = router->newSeqnum();
	seqnumTable[seq] = 0;

	packet->seqnum = seq; // devra faire l'incrémentation'
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
		mess__free(&packet);
		throw;
	}


	if (network__send(c, mess__toString(packet)) < 0)
	{
		std::cerr << "[CONTROLLER] Error while sending message '" << mess__toString(packet)
				  << "' to '" <<  c->id << "' on socket '" << (int)c->sock << "'\n";
		network__debug(router->getNetwork());
	}

	mess__free(&packet);
}


void PromptActions::ping(Message* mess, std::map<int, hdclock::time_point>& pingTimeTables)
{
	struct Message* ll_ping; // ping de bas niveau (low level)
	mess__init(&ll_ping);

	ll_ping->type = PING;

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
		mess__free(&ll_ping);
		throw;
	}

	for (int i = 0; i < router->getConfiguration()->defaultPingPacketCount; i++)
	{
		ll_ping->seqnum = router->newSeqnum();
		pingTimeTables[ll_ping->seqnum] = hdclock::now();

		network__send(c, mess__toString(ll_ping)); //segfault tant qu'on a pas un vrai c
		sleep(1);
	}

	mess__free(&ll_ping);
}


void PromptActions::route(Message* mess)
{
	struct Message* ll_ping; // ping de bas niveau (low level)
	mess__init(&ll_ping);

	ll_ping->type = PING;

	ll_ping->node1 = strcopy(router->getName());
	ll_ping->node2 = strcopy(mess->node1);
	ll_ping->n_parameter = 0;
	// on regarde le next hop dans la table de routage
	Client* c = 0;

	try
	{
		c = router->getRouteTable().nextClient(std::string(ll_ping->node2));
	}
	catch(UnknownDest)
	{
		mess__free(&ll_ping);
		throw;
	}

	int i = 0;
	while(i < 10) // max ttl ? temps ? tant qu'on n'a pas reçu de réponse favorable à notre route ?
	//peut être exécuter dans un thread avec incrémentation et comparaison d'une variable dans la classe (mutex)
	{
		ll_ping->n_parameter = i++;
		ll_ping->seqnum = router->newSeqnum();
		network__send(c, mess__toString(ll_ping));
		sleep(1);
	}

	mess__free(&ll_ping);
}
