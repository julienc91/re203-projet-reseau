#include "prompt_actions.hpp"
#include "exceptions.hpp"
#include <cstring>

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
	// on regarde le next hop dans la table de routage  (router->getRoutetable ou qqch comme ça)
	Client* c = 0; // = ... celui qui correspond à mess->node1

	// si pas dans table de routage, erreur.
	// throw unknownDest;



	// c = quelquechose...
	// on transforme le message en packet
	struct Message* packet;
	mess__init(&packet);

	packet->seqnum = router->newSeqnum(); // devra faire l'incrémentation'
	packet->node1 = strcopy(router->getName()); //source
	packet->node2 = strcopy(mess->node1); // destination
	packet->n_parameter = router->getConfiguration()->defaultTTLValue;
	packet->s_parameter = strcopy(mess->s_parameter);

	// network__send(c, mess__toString(packet)); //segfault tant qu'on a pas un vrai c

	// note : le treatOutput doit être fait dans net.h
}


void PromptActions::ping(Message* mess)
{
	// on regarde le next hop dans la table de routage  (router->getRoutetable ou qqch comme ça)

	Client* c = 0; // = ... celui qui correspond à mess->node1

	// si pas dans table de routage, erreur.
	// throw unknownDest;

	struct Message* ll_ping; // ping de bas niveau (low level)
	mess__init(&ll_ping);
	// ping seqnum N src id dst id ttl val*

	ll_ping->seqnum = router->newSeqnum();
	ll_ping->node1 = strcopy(router->getName());
	ll_ping->node2 = strcopy(mess->node1);
	ll_ping->n_parameter = router->getConfiguration()->defaultTTLValue;

	for (int i = 0; i < router->getConfiguration()->defaultPingPacketCount; i++)
	{
		// network__send(c, mess__toString(packet)); //segfault tant qu'on a pas un vrai c
	}
}


void PromptActions::route(Message* mess)
{
	// on regarde le next hop dans la table de routage  (router->getRoutetable ou qqch comme ça)

	Client* c = 0; // = ... celui qui correspond à mess->node1

	// si pas dans table de routage, erreur.
	// throw unknownDest;

	struct Message* ll_ping; // ping de bas niveau (low level)
	mess__init(&ll_ping);
	// ping seqnum N src id dst id ttl val*

	ll_ping->seqnum = router->newSeqnum();
	ll_ping->node1 = strcopy(router->getName());
	ll_ping->node2 = strcopy(mess->node1);

	int i = 1;
	while(i < 100) // max ttl ? temps ? tant qu'on n'a pas reçu de réponse favorable à notre route ?
	{
		ll_ping->n_parameter = i;
		// network__send(c, mess__toString(packet)); //segfault tant qu'on a pas un vrai c
	}
}
