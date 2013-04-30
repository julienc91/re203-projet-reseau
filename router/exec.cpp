#include "display.hpp"
#include "exec.hpp"
#include "exceptions.hpp"
#include "net_actions.hpp"
#include <iostream>

void Exec::prompt_message(Message* m)
{
	NetActions action;
	Display disp;

	if(m == NULL)
	{
		return;
	}

	switch(m->type)
	{
		case MESSAGE:
			//calcul temps

			//actions sur réseau
			// il faut séparer le message en packet de bonne taille et les envoyer
			try
			{
				action.message(m);
			}
			catch(UnknownDest&)
			{
				disp.err_unknown();
				return;
			}

			//affichage
			disp.mess_sent();
			//attendre le retour
			//while(pas de retour || !timeout dépassé)
			//disp.mess_not_deliv(router->getConfiguration()->defaultPacketTimeoutValue);
			break;

		case PING:
			//actions sur réseau
			action.ping(m);

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
