#include "display.h"
#include "exec.h"
#include <iostream>
#include <ctime>

void Exec::prompt_message(Message* m)
{
	if(m == NULL)
	{
		return;
	}

	switch(m->type)
	{
		case MESSAGE:
			//calcul temps
			time_t stamp = time(0);

			//actions sur réseau

			//affichage

			break;

		case PING:
			// actions sur graphe

			//actions sur réseau

			//affichage
			break;

		case ROUTE:
			// actions sur graphe

			//actions sur réseau

			//affichage
			break;

		case ROUTETABLE:
			// actions sur graphe

			//actions sur réseau

			//affichage
			break;

		default:
			std::cout << "ERREUR: Action non reconnue" << std::endl;
			break;
	}
}


void Exec::sock_message(Message* m)
{
	if(m == NULL)
	{
		return;
	}

	switch(m->type)
	{
		case GREETING:
			//actions sur le graphe

			break;

		case NEIGHBORHOOD:
			///mettre à jour la topologie si nécessaire

			break;

		case BYE:
			break;

		//**** routeur <-> routeur ****//
		case LINK:
			if(m)
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
			printf("ERREUR: Action non reconnue");
			break;
	}
}
