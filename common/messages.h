#include <stddef.h>
enum MessageType
{
	//Controleur
	LOAD, 			//load fichier
	SAVE, 			//save fichier
	SHOW, 			//show topology
	ADDLINK, 		//add link n1 n2 w
	UPDATELINK, 	//update link n1 n2 w
	DELLINK, 		//del link n1 n2
	DISCONNECT, 	//disconnect n

	//Routeur
	MESSAGE, 		//message n mess
	//PING, 		//ping n (est fusionné avec le ping inter-terminaux)
	ROUTE, 			//route n
	ROUTETABLE, 	//routetable

	//Protocole (tout se finit par *)
	LOGIN, 			//log in as ID port p
					//log in port p
	LOGOUT, 		//log out
	GREETING, 		//greeting n  (que se passe-t-il si rien libre?)
	BYE,			//bye

	POLL,			//poll
	NEIGHBORHOOD,	//neighborhood newlist [...;id_i,cost_i,ip_i:port_i;...]
					//neighborhood ok

	LINK,			//link
					//link ok
	VECTOR,			//vector [..]
					//vector ok
	PACKET,			//packet src idSrc dst idDst ttl x data ...
					//packet src idSrc dst idDst ok
					//packet src idSrc dst idDst toofar
	PING,			//ping src id dst id ttl val*
	PONG,			//pong src id dst id ttl val*

	//Wildcard
	NONE
};


struct Message
{
	enum MessageType type;
	char *s_parameter;
	char *node1, *node2;
	int n_parameter1, n_parameter2;
	int ok;
};

void mess__init(struct Message** mess);
void mess__parse(struct Message* mess_dest, char* mess_src, size_t len);
