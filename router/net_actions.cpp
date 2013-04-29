#include "net_actions.hpp"
#include "exceptions.hpp"
#include <cstring>

extern "C"
{
	#include "../common/util.h"
}




void NetActions::message(Message* mess)
{

	// on regarde le next hop dans la table de routage
	Client* c; // = ...

	// si pas dans table de routage, erreur.
	// throw unknownDest;



	// c = quelquechose...
	// on transforme le message en packet
	struct Message* packet;
	mess__init(&packet);

//	packet->seqnum = router->newSeqnum(); // devra faire l'incrémentation'
//	strcopy(packet->node1, router->getName()); //source
	packet->node2 = strcopy(mess->node1); // destination
//	packet->n_parameter = router->getConfiguration()->defaultTTLValue;
	packet->s_parameter = strcopy(mess->s_parameter);

	network__send(c, mess__toString(packet));
}


void NetActions::ping(Message* mess)
{

}
