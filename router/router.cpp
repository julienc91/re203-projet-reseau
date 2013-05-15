#include "router.hpp"
#include "prompt_actions.hpp"
#include "sock_actions.hpp"
#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <mutex>
#include "display.hpp"

Router* glob__router = 0;

/** \brief initialize the instance of Router
 *  \param name name of the router
 *  \param conf name of the file to load the configuration
 */

Router::Router(char* name, char* conf)
{
	_isRunning = true;
    _name = new std::string(name);
    seqnum = 0;

   	// * * * * lecture fichier config * * * *
	config = config__readRouter(conf);

	exec = new Exec(this);
	paction = new PromptActions(this);
	saction = new SockActions(this);
	glob__router = this;

	init(); // windows compatibility

	std::chrono::seconds controllerTimeout( getConfiguration()->controllerUpdateInterval );

	// * * * * ouverture serveur * * * *
	net = network__open(config->routerPort);

	// * * * * evenements * * * *
	net->input_event =  Event::input;
	net->connection_event = Event::connect;
	net->disconnection_event = Event::disconnect;
	net->message_event = Event::message;

	// * * * * connexion sortante * * * *
	do
	{
		std::cout << "Connection to controller : "<< config->controllerAddress << ":" << config->controllerPort << std::endl;
		controller = network__connect(net, config->controllerAddress, config->controllerPort);
		if(!controller)
			std::this_thread::sleep_for(controllerTimeout);
	} while(!controller);


	strcpy(controller->id, "controlleur");
	std::cout << "Connected on socket " << controller->sock << std::endl;

	sockActions()->login(config->routerPort, name);


	// * * * * gestion stdin   * * * *
	void (Exec::*meth)(Message* m) = &Exec::prompt_message;
	prompt.start(exec, meth);

	// début
	runControllerLoop = true;
	runRouterLoop = true;
	runMainLoop = true;

	mainLoopThread = new std::thread(&Router::mainLoop, this);
	controllerLoopThread = new std::thread(&Router::controllerLoop, this);
	routerLoopThread = new std::thread(&Router::routerLoop, this);
}

/** \brief intitialize the instance of Router by copying data
 * 	\param source instance of Router
 */

Router::Router(const Router * data)
{
    _name   = data->_name;
}

Router::~Router()
{
	// Fermeture stdio
	prompt.stop();

	runControllerLoop = false;
	runRouterLoop = false;
	runMainLoop = false;
	// * * * * fermeture serveur * * * *

	network__close(net);
	network__free(net);

	// threads
    
    std::cerr << "#1#" << std::endl;

	mainLoopThread->detach(); // trop long avec le timeout
	routerLoopThread->join();
    
        std::cerr << "#2#" << std::endl;
        
	controllerLoopThread->join();
    
        std::cerr << "#3#" << std::endl;

	delete routerLoopThread;
	delete controllerLoopThread;
	delete mainLoopThread;

	end(); // windows compatibility

	free(config);
    delete _name;
    delete exec;
    delete saction;
    delete paction;
}


/** \brief set router to non running state
 */
void Router::stop()
{
	_isRunning = false;
}

/** \brief fonction to call to run the router
 */
void Router::mainLoop()
{
	// * * * * gestion serveur * * * *
	while(network__is_opened(net) && runMainLoop)
	{
		network__update(net);
	}
}

/** \brief
 */
void Router::controllerLoop()
{
	std::chrono::seconds poll_time(getConfiguration()->controllerUpdateInterval );
	std::this_thread::sleep_for(std::chrono::seconds(1));
	while(runControllerLoop)
	{
		sockActions()->poll();
		std::this_thread::sleep_for(poll_time); // j'aime quand on peut lire le code
	}
}

/** \brief
 */

void Router::routerLoop()
{
	std::chrono::seconds vect_time( getConfiguration()->routerUpdateInterval );
	std::this_thread::sleep_for(std::chrono::seconds(1));
	while(runRouterLoop)
	{
		for(RouteTable::iterator i = routeTable.begin(); i != routeTable.end(); i++)
		{
			if((*i).second.isNeighbor() && (*i).second.isComplete())
			{
				char * vect_str = routeTable.vector((*i).first);
				//std::cout << "\nici\n" << (*i).first << " " << (*i).second.client()->id << "\n";
				if(strcmp(vect_str, "[]") != 0)
					sockActions()->vector((char*) (*i).first.c_str(), vect_str);
			}
		}
		std::this_thread::sleep_for(vect_time);
	}
}

/** \brief increments the number of seqnum
 * 	\return new seqnum total
*/
int Router::newSeqnum()
{
	return ++seqnum;
}

/** \brief get the name of the router
 *  \return the name of the router (char *)
 */
char* Router::getName()
{
	return (char*) _name->c_str();
}

/** \brief set a new name for the router
 */
void Router::setName(char* name)
{
	delete _name;
	_name = new std::string(name);
}

/** \return the configuration of the router
 */
Configuration* Router::getConfiguration()
{
	return config;
}

/** \return the routetable of the router
 */
RouteTable& Router::getRouteTable()
{
	return routeTable;
}


Client* Router::getController()
{
	return controller;
}


PromptActions* Router::promptActions()
{
	return paction;
}

SockActions* Router::sockActions()
{
	return saction;
}

/** \brief parse the str_orig strig to read and save
 * information about the curent state of the network
 * 	\param str_orig string to parse (char *)
 * 	\param node_orig node which sent str_orig
 */
void Router::parseVector(char* str_orig, char* node_orig)
{
	std::string sourceNode(node_orig);
	char *str = strcopy(str_orig + 1); // pour le [
	str[strlen(str) - 1] = 0; // pour le ]

	std::vector<char*> v;
	// 1 : on extrait chaque groupe d'infos (séparés par ;)
	char * r = strtok(str, ";");
	if(r != NULL)
		v.push_back(r);

	while((r = strtok(NULL, ";")) != NULL)
	{
		v.push_back(r);
	}

	// dans le tableau on a id,dist  dans chaque case.
	// dans la routetable, il faut :
	// si le noeud est présent, mettre à jour la distance si elle est plus courte
	// sinon l'ajouter et mettre en next hop sourceNode

	std::vector<char*>::iterator i;

	// On ajoute ceux qui ne sont pas dans la table
	for(i = v.begin(); i != v.end(); ++i)
	{
		std::string s(strtok((*i), ","));
		if(s != std::string(getName()))
		{
			int dist = atoi(strtok(NULL, ","));

			if(routeTable.find(s) != routeTable.end()) // si on le trouve
			{
				if((routeTable[s].isNeighbor() && dist < routeTable[s].dist()) ||
                    (!routeTable[s].isNeighbor() && dist + routeTable[sourceNode].dist() < routeTable[s].dist()) ||
                    (!routeTable[s].isNeighbor() && routeTable[s].nextHop() == sourceNode &&
                                                    routeTable[s].dist() != dist + routeTable[sourceNode].dist()) ||
					(routeTable[s].dist() < 0))
				{

					routeTable[s].nextHop() = sourceNode;
					if(!routeTable[s].isNeighbor())
					{
						routeTable[s].dist() = dist + routeTable[sourceNode].dist();
					}
					else
					{
						routeTable[s].dist() = dist;
					}

				}
			}
			else
			{
				routeTable[s] = Entry(s, sourceNode, dist + routeTable[sourceNode].dist());
				// exception si sourceNode pas dans hashtable ? peu probable
			}
		}
	}
}
// [a, b, c]



void Router::parseNeighborhood(char* str_orig)
{
	char *str = strcopy(str_orig + 1); // pour le [
	str[strlen(str) - 1] = 0; // pour le ]

	if(strcmp(str, "") != 0)
	{
		std::vector<char*> v;
		// 1 : on extrait chaque groupe d'infos (séparés par ;)
		char * r = strtok(str, ";");
		if(r != NULL)
			v.push_back(r);

		while((r = strtok(NULL, ";")) != NULL)
		{
			v.push_back(r);
		}



		// à ce moment, on a dans v : | a,b,c:d  | e,f,g:h |  par ex.
		// (si vect = [a,b,c,d;e,f,g,h;...])

		std::vector<char*>::iterator i;
		std::vector<std::string> routerNames;

		// On ajoute ceux qui ne sont pas dans la table
		for(i = v.begin(); i != v.end(); ++i)
		{
			std::string s(strtok((*i), ","));
			routerNames.push_back(s);
			if(s.compare(std::string(getName())) != 0)
			{
				if(routeTable.find(s) == routeTable.end())
				{
                    std::cerr << "CLIENT DANS NEIGHBORHOOD ET PAS DANS LA TABLE !!!"<< std::endl;
					routeTable[s] = Entry(s, s, atoi(strtok(NULL, ",")));

					char * ip = strtok(strtok(NULL, ","), ":");
					int port = atoi(strtok(NULL, ":"));
					Client *c = network__connect(net, ip, port);
					strcpy(c->id, s.c_str());
					routeTable[s].setClient(c);
					routeTable[s].isNeighbor() = true;
					routeTable[s].isComplete() = true;

					this->saction->link(c);
				}
				else if(!routeTable[s].isComplete())
				{
                    std::cerr << "CLIENT DANS NEIGHBORHOOD, DANS LA TABLE MAIS INCOMPLET!!!"<< std::endl;
                    std::cerr << "<<" <<  routeTable[s].client()->id <<  ">>"<< std::endl;
                    
					routeTable[s].dist() = atoi(strtok(NULL, ","));
					//~ char * ip = strtok(strtok(NULL, ","), ":");
					//~ int port = atoi(strtok(NULL, ":"));
					//~ Client *c = network__connect(net, ip, port);
					//~ strcpy(c->id, s.c_str());
					//~ routeTable[s].setClient(c);
					routeTable[s].isNeighbor() = true;
					routeTable[s].isComplete() = true;
				}
				else
				{
					routeTable[s].dist() = atoi(strtok(NULL, ","));
					strtok(strtok(NULL, ","), ":");
					strtok(NULL, ":");
				}
			}
		}


		RouteTable::iterator k;
		// On enlève ceux qui n'y sont plus
		for(k = routeTable.begin(); k != routeTable.end(); k++)
		{
			if(std::find(routerNames.begin(), routerNames.end(), (*k).first) == routerNames.end() && routeTable[(*k).first].isNeighbor())
			{
				network__disconnect(net, (*k).second.client());
				routeTable.erase(k);
			}
			// TODO enlever ceux qui ont en first hop un de ceux qui viennent d'être enlevés
		}
	}
	else // on vide la table de routage
	{
		routeTable.clear();
	}
	free(str);
}

network* Router::getNetwork()
{
	return this->net;
}

bool Router::isRunning()
{
	return _isRunning;
}
