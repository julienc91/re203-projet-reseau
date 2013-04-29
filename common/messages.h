#ifndef __MESSAGES_H_
#define __MESSAGES_H_

enum Acceptance
{
	NOT,
	OK,
	TOOFAR
};

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
	// Routeur <-> Controleur 
	LOGIN, 			//log in as ID port p
					//log in port p
	LOGOUT, 		//log out
	GREETING, 		//greeting n  (que se passe-t-il si rien libre?)
	BYE,			//bye

	POLL,			//poll
	NEIGHBORHOOD,	//neighborhood newlist [...;id_i,cost_i,ip_i:port_i;...]
					//neighborhood ok

	// Routeur <-> Routeur
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
	int n_parameter;
	enum Acceptance accept;
};

void mess__init(struct Message** mess);
void mess__free(struct Message** mess);

struct Message* mess__parse(char* mess_src);

char* mess__escape(char* mess_src);
char* mess__unescape(char* mess_src);

int mess__getAcceptance(struct Message* mess);
int mess__getTTL(struct Message* mess);
int mess__getWeight(struct Message* mess);

char* mess__treatInput(char * src);
char* mess__treatOutput(char * src);

char* mess__toString(struct Message* mess);

void mess__debug(struct Message* m);
#endif
