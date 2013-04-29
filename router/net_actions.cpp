#include "net_actions.hpp"
#include "exceptions.hpp"
#include <cstring>

extern "C"
{
	#include "../common/util.h"
}




void NetActions::message(Message* mess)
{
	Client* c;
	// on regarde le next hop dans la table de routage


	// si pas dans table de routage, erreur.
	// throw unknownDest;



	// c = quelquechose...
	// on transforme le message en packet
	struct Message* packet;
	packet = mess__init();

	packet->seqnum = router->newSeqnum(); // devra faire l'incrémentation'
	strcopy(packet->node1, router->getName()); //source
	strcopy(packet->node2, mess->node1); // destination
	packet->n_parameter = router->getConfiguration()->defaultTTLValue;
	strcopy(packet->s_parameter, mess->s_parameter);

	network__send(c, mess__toString(packet));
}
