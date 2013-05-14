#include <iostream>
#include "exec.hpp"
#include "display.hpp"
#include "exceptions.hpp"
#include "prompt_actions.hpp"
#include "sock_actions.hpp"


#include <cstring>
#include <cstdlib>
#include <climits>

Exec::Exec(Router* r)
{
	router = r;
	isWaitingForRoute = false;
	routeDest = 0;
	routeCount = 0;
	pingCount = 0;

	pingMin  = INT_MAX;
	pingMax  = INT_MIN;
	pingAvg  = 0;
	pingSucc = 0;
	threadRunning = true;
	disp = new Display();

	//timeCheckerThread = new std::thread(&Exec::timeChecker, this);
}

Exec::~Exec()
{
	threadRunning = false;
	delete disp;
	timeCheckerThread->join();
}

void Exec::timeChecker()
{
	std::chrono::seconds oneSecond(1);

	while(threadRunning)
	{
		std::this_thread::sleep_for(oneSecond);
		// incrémentation
		std::map<int, int>::iterator i_map;
		for(i_map = messageTimes.begin(); i_map != messageTimes.end(); ++i_map)
		{
			if(++(*i_map).second > router->getConfiguration()->defaultPacketTimeoutValue) //TODO mutex sur le ++
			{
				messageTimes.erase(i_map);
				disp->err_unreachable();
			}
		}

		RouteTable::iterator i_rt;
		for(i_rt = router->getRouteTable().begin(); i_rt != router->getRouteTable().end(); ++i_rt)
		{
			if(++(*i_rt).second.secondsInactive() > router->getConfiguration()->defaultDVTimeoutValue) //TODO mutex sur le ++
			{
				std::cout << "glop\n";
				network__disconnect(router->getNetwork(), (*i_rt).second.client());
				router->getRouteTable().erase(i_rt);
			}
		}
	}
}

void Exec::prompt_message(Message* m)
{
	if(m == NULL || m->type == NONE)
	{
		return;
	}

	switch(m->type)
	{
		case POLL:
			router->sockActions()->poll();
			break;

		case MESSAGE:
			try
			{
				messageTime = hdclock::now();
				router->promptActions()->message(m, messageTimes);
			}
			catch(UnknownDest&)
			{
				disp->err_unknown();
				return;
			}

			disp->mess_sent();
			break;

		case PING:
			try
			{
				pingCount = router->getConfiguration()->defaultPingPacketCount;
				router->promptActions()->ping(m, pingTimeTables);
			}
			catch(UnknownDest&)
			{
				pingCount = 0;
				disp->err_unknown();
				return;
			}
			catch(HostUnreachable&)
			{
				pingCount = 0;
				disp->err_unreachable();
				return;
			}

			//affichage se fait à la réception des pong
			break;

		case ROUTE:
			//actions sur réseau
			try
			{
				routeCount = 0;
				isWaitingForRoute = true;
				routeDest = strcopy(m->node1);
				disp->route_init(router->getName(), m->node1);
				router->promptActions()->route(m);
			}
			catch(UnknownDest&)
			{
				isWaitingForRoute = false;
				disp->err_unknown();
				return;
			}
			break;

		case ROUTETABLE:
			//affichage
			disp->routetable(router->getRouteTable());
			break;

        case NETWORK_DEBUG:
            network__debug(router->getNetwork());
            break;

		case QUIT:
			router->sockActions()->logout();
            std::cerr<<"[ROUTER] Closed."<<std::endl;
            exit(0);
			break;

		default:
			std::cout << "[ERREUR] Action non reconnue" << std::endl;
			break;
	}
}


void Exec::sock_message(Message* m, Client* t)
{
	if(m == NULL || m->type == NONE)
	{
		return;
	}

	switch(m->type)
	{
		case GREETING:
			router->setName(m->node1);
			disp->loginMessage(m->node1);
			break;

		case NEIGHBORHOOD:
			if(m->accept == NOT)
			{
				router->parseNeighborhood(m->s_parameter);
			}
			// sinon ne rien faire, ça ne change pas
			break;

		case BYE:
			// quitter le logiciel
			router->stop();
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
				std::string s(m->node1);
				if(router->getRouteTable().find(s) == router->getRouteTable().end())
				{
					router->getRouteTable()[s] = Entry(s, s, INT_MIN);
					std::cout << "nom du client dans link " << m->node1 << " ip : " << client__get_address(t) << ":" << client__get_port(t) <<"\n" ;
					strcpy(t->id, m->node1);
					router->getRouteTable()[s].isNeighbor() = true;
					//router->getRouteTable()[s].setClient(t);
				}
				router->sockActions()->linkAck(t);
			}
			break;

		case VECTOR:
			if(m->accept == OK) // cool bro
			{
				// potentiellement rien à faire
				router->getRouteTable()[std::string(t->id)].secondsInactive() = 0;
			}
			else
			{
				router->parseVector(m->s_parameter, t->id);
				router->sockActions()->vectorAck(t->id);
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
					messageTimes.erase(m->seqnum);
					disp->mess_deliv(std::chrono::duration_cast<milliseconds>(hdclock::now() - messageTime).count());
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
					int msDuration = std::chrono::duration_cast<milliseconds>(hdclock::now() - pingTimeTables[m->seqnum]).count();
					if(msDuration > pingMax)
					{
						pingMax = msDuration;
					}
					else if(msDuration < pingMin)
					{
						pingMin = msDuration;
					}

					pingSucc += 1;
					pingAvg += msDuration;
					pingCount--;
					disp->ping_echo(m->node2, m->node1, msDuration); // à changer
					pingTimeTables.erase(m->seqnum);


					if(pingCount == 0)
					{
						disp->ping_result(pingSucc * 100 / router->getConfiguration()->defaultPingPacketCount,
											100 - pingSucc * 100 / router->getConfiguration()->defaultPingPacketCount,
											pingMin,
											pingAvg / router->getConfiguration()->defaultPingPacketCount,
											pingMax);

						pingMin = INT_MAX;
						pingMax = INT_MIN;
						pingAvg = 0;
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
