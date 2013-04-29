#include "display.h"
#include "exec.h"
#include <iostream>
#include <ctime>

void Exec::prompt_message(Message* m)
{
	//time_t stamp;
	if(m == NULL)
	{
		return;
	}

	switch(m->type)
	{
		case MESSAGE:
			//calcul temps
		//	stamp = time(0);

			//actions sur réseau
			// il faut séparer le message en packet de bonne taille et les envoyer

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
			std::cout << "link reçu" << std::endl;
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
			std::cout << "ERREUR: Action non reconnue" << std::endl;
			break;
	}
}
