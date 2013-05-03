#include <iostream>
#include "exec.hpp"
#include "display.hpp"
#include "exceptions.hpp"
#include "prompt_actions.hpp"
#include "sock_actions.hpp"

#include <cstring>
#include <cstdlib>

Exec::Exec(Router* r)
{
	router = r;
	isWaitingForRoute = false;
	routeDest = 0;
	routeCount = 0;
	pingCount = 0;
	pingTimeTables = new time_t[router->getConfiguration()->defaultPingPacketCount];

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
				router->promptActions()->message(m);
			}
			catch(UnknownDest&)
			{
				disp->err_unknown();
				return;
			}

			disp->mess_sent();

			//attendre le retour (dans un thread)
			//while(pas de retour || !timeout dépassé)
				//disp.mess_not_deliv(router->getConfiguration()->defaultPacketTimeoutValue);
			break;

		case PING:
			try
			{
				router->promptActions()->ping(m);
				pingCount = router->getConfiguration()->defaultPingPacketCount;
			}
			catch(UnknownDest&)
			{
				disp->err_unknown();
				return;
			}
			catch(HostUnreachable&)
			{
				disp->err_unreachable();
				return;
			}

			//affichage se fait à la réception des pong
			break;

		case ROUTE:
			//actions sur réseau
			try
			{
				router->promptActions()->route(m);
				isWaitingForRoute = true;
				routeDest = strcopy(m->node2);
				routeCount = 0;

				disp->route_init(m->node1, m->node2);
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
			std::cout << "TODO" << std::endl; // TODO
			break;

		default:
			std::cout << "ERREUR: Action non reconnue" << std::endl;
			break;
	}
}


void Exec::sock_message(Message* m)
{
	if(m == NULL || m->type == NONE)
	{
		return;
	}

	switch(m->type)
	{
		case GREETING:
			router->setName(m->node1);
			break;

		case NEIGHBORHOOD:
			if(m->accept == NOT)
			{
				//charger la liste
				//traiter (format de donnée : TODO )
				//mettre à jour la topologie si nécessaire
			}
			// sinon ne rien faire, ça ne change pas
			break;

		case BYE:
			// quitter le logiciel
			exit(0);
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
					// prendre la timestamp et ...
					disp->mess_deliv(0); // TODO
				}
				else if(m->accept == TOOFAR)
				{
					// on n'a pas pu envoyer le paquet
					disp->err_unreachable();
				}
				else // cas ou on reçoit un paquet qui nous est destiné
				{
					// on envoie l'acquittement
					m->accept = OK;
					router->sockActions()->reverse(m); //seqnum ne change pas!!!!!!
					disp->mess_received(m->s_parameter);
				}
			}
			else // on fait juste transiter un paquet
			{
				//dec. TTL
				if(mess__getAndDecTTL(m) != 0)
				{
					// envoyer (p-ê rajouter gestion d'exceptions ici)
					router->sockActions()->forward(m);
				}
				else
				{
					m->accept = TOOFAR;
					//renvoyer dans l'autre sens
					router->sockActions()->reverse(m);
				}
			}
			break;
		case PING:
			// 1 : on regarde la destination et on compare avec nous
			if(strcmp(m->node2, router->getName()) == 0)
			{
				//envoyer pong
				m->type = PONG;
				router->sockActions()->reverse(m);
			}
			else // on fait transiter
			{
				if(mess__getAndDecTTL(m) != 0)
				{
					router->sockActions()->forward(m);
				}
				else
				{
					m->accept = TTLZERO;
					//renvoyer dans l'autre sens
					router->sockActions()->reverse(m);
				}
			}

			break;

		case PONG:
			// 1 : on regarde la destination et on compare avec nous
			if(strcmp(m->node2, router->getName()) == 0)
			{
				// ici, pb si on envoie ping puis route  (ou deux pings)juste après.
				// Nécessité : système de ports virtuels (deux applications différentes)

				// ou bein faire la différence avec une hashmap de seqnum ?
				if(pingCount > 0)
				{
					pingCount--;
					disp->ping_echo(m->node1, m->node2, *pingTimeTables); // à changer

					if(pingCount == 0)
					{
						// taux de succès: TODO
						double success=0;
						double failure=0;
						double min=0;
						double avg=0;
						double max=0;
						disp->ping_result(success, failure, min, avg, max);
					}
				}
				else if(isWaitingForRoute)
				{
					routeCount++;
					disp->route_hop(routeCount, m->node1);

					//si on a atteint notre cible...
					if(strcmp(routeDest, m->node1) == 0)
					{
						isWaitingForRoute = false;
						disp->route_result(routeCount, routeTime);
					}
				}
			}
			else
			{
				if(mess__getAndDecTTL(m) != 0)
				{
					router->sockActions()->forward(m);
				}
				// ne rien faire si le ttl arrive à 0 sur le chemin du retour...
			}
			break;

		default:
			std::cout << "ERREUR: Action non reconnue" << std::endl;
			break;
	}
}
