#include <iostream>
#include "exec.hpp"
#include "display.hpp"
#include "exceptions.hpp"
#include "prompt_actions.hpp"

Exec::Exec(Router* r)
{
	router = r;
	paction = new PromptActions(r);
	disp = new Display();
}

void Exec::prompt_message(Message* m)
{
	if(m == NULL || m->type == NONE)
	{
		return;
	}

	switch(m->type)
	{
		case MESSAGE:
			try
			{
				paction->message(m);
			}
			catch(UnknownDest&)
			{
				disp->err_unknown();
				return;
			}

			disp->mess_sent();

			//attendre le retour
			//while(pas de retour || !timeout dépassé)
				//disp.mess_not_deliv(router->getConfiguration()->defaultPacketTimeoutValue);
			break;

		case PING:
			try
			{
				paction->ping(m);
			}
			catch(UnknownDest&)
			{
				disp->err_unknown();
				return;
			}
			catch(HostUnreachable&)
			{
				disp->err_unreachable();
			}

			//affichage se fait à la réception des pong

			break;

		case ROUTE:
			//actions sur réseau
			try
			{
				paction->route(m);
			}
			catch(UnknownDest&)
			{
				disp->err_unknown();
				return;
			}
			break;

		case ROUTETABLE:
			//affichage
			//disp->routetable(router->getRoutetable());
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
		//	router->setName(m->node1);
		// il faudra penser à mettre à jour le nom du routeur partout ou il est stocké
			break;

		case NEIGHBORHOOD:
			///mettre à jour la topologie si nécessaire
			if(m->accept == NONE)
			{
				//charger la liste
			}
			break;

		case BYE:
			// quitter le logiciel
			break;

		//**** routeur <-> routeur ****//
		case LINK:
			if(m->accept == OK) // on recoit la réponse de celui avec lequel on a voulu créer un lien
			{
				// p-ê faire opérations sur hashtable ? marquer l'autre comme actif ?
			}
			else // on demande de créer un lien
			{
				// de même avec l'autre (on doit avoir le client d'ou ça vient ! sinon message pas très utile...)
			}
			break;

		case VECTOR:
			if(m->accept == OK) // cool bro
			{
				// potentiellement rien à faire
			}
			else
			{
				// on met à jour la hashtable
			}
			break;

		case PACKET:
			// 1 : on regarde la destination et on compare avec nous
			if(strcmp(m->node2, router->getName()) == 0)
			{
				// 2 : si c'est nous :
				if(m->accept == OK)
				{
					// cool, tout s'est bien passé
				}
				else if(m->accept == TOOFAR)
				{
					// on n'a pas pu envoyer le paquet
				}
				else // cas ou on reçoit un paquet qui nous est destiné
				{
					// on envoie l'acquittement'
				}
			}
			else // on fait juste transiter un paquet
			{
				//dec. TTL
				if(--(m->n_parameter) != 0)
				{
					// chercher à qui envoyer dans la routetable

					// envoyer si possible
				}
				else
				{
					m->accept = TTLZERO;
					//renvoyer dans l'autre sens
				}
			}
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
