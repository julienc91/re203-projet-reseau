#include "router.hpp"
#include "prompt_actions.hpp"
#include "sock_actions.hpp"
#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <chrono>

Router* glob__router = 0;

Router::Router(char* name, char* conf)
{
    _name = new std::string(name);
    seqnum = 0;

   	// * * * * lecture fichier config * * * *
	config = config__readRouter(conf);

	exec = new Exec(this);
	paction = new PromptActions(this);
	saction = new SockActions(this);
	glob__router = this;

	init(); // windows compatibility


	// * * * * ouverture serveur * * * *
	net = network__open(config->routerPort);

	// * * * * evenements * * * *
	net->input_event =  Event::input;
	net->connection_event = Event::connect;
	net->disconnection_event = Event::disconnect;
	net->message_event = Event::message;

	// * * * * connexion sortante * * * *
	std::cout << "Connection to controller : "<< config->controllerAddress << ":" << config->controllerPort << std::endl;
	controller = network__connect(net, config->controllerAddress, config->controllerPort); //localhost à changer

	if (!controller)
	{
		std::cerr << "Connection failed." << std::endl;
	}
	else
	{
		strcpy(controller->id, "controlleur");
		std::cout << "Connected on socket " << controller->sock << std::endl;

		sockActions()->login(config->routerPort, name);
	}

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

	label:
		goto label;
}

Router::Router(const Router * data)
{
    _name   = data->_name;
}

Router::~Router()
{
	// * * * * fermeture serveur * * * *

	network__close(net);
	network__free(net);

	end(); // windows compatibility

	runControllerLoop = false;
	runRouterLoop = false;
	runMainLoop = false;
	routerLoopThread->join();
	controllerLoopThread->join();
	mainLoopThread->join();
    delete _name;
    delete exec;
    delete saction;
    delete paction;
}

void Router::mainLoop()
{
	// * * * * gestion serveur * * * *
	while(network__is_opened(net) && runRouterLoop)
	{
		network__update(net);
	}
}

void Router::controllerLoop()
{
	std::chrono::seconds poll_time( getConfiguration()->controllerUpdateInterval );
	while(runControllerLoop)
	{
		std::this_thread::sleep_for(poll_time); // j'aime quand on peut lire le code
		sockActions()->poll();
	}
}

void Router::routerLoop()
{
	std::chrono::seconds vect_time( getConfiguration()->routerUpdateInterval );
	while(runRouterLoop)
	{
		std::this_thread::sleep_for(vect_time); // j'aime quand on peut lire le code

		char * vect_str = routeTable.vector();
		for(RouteTable::iterator i = routeTable.begin(); i != routeTable.end(); i++)
			sockActions()->vector((char*) (*i).first.c_str(), vect_str);
	}
}

int Router::newSeqnum()
{
	return ++seqnum;
}
char* Router::getName()
{
	return (char*) _name->c_str();
}

void Router::setName(char* name)
{
	delete _name;
	_name = new std::string(name);
}

Configuration* Router::getConfiguration()
{
	return config;
}

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
		int dist = atoi(strtok(NULL, ","));

		if(routeTable.find(s) != routeTable.end())
		{
			if(dist < routeTable[s].dist())
			{
				routeTable[s].dist() = dist;
				routeTable[s].nextHop() = sourceNode;
			}
		}
		else
		{
			routeTable[s] = Entry(s, sourceNode, dist + routeTable[sourceNode].dist());
			// exception si sourceNode pas dans hashtable ? peu probable
		}
	}
}

void Router::parseNeighborhood(char* str_orig)
{
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

	// à ce moment, on a dans v : | a,b,c,d  | e,f,g,h |  par ex.
	// (si vect = [a,b,c,d;e,f,g,h;...])

	std::vector<char*>::iterator i;
	std::vector<std::string> routerNames;

	// On ajoute ceux qui ne sont pas dans la table
	for(i = v.begin(); i != v.end(); ++i)
	{
		std::string s(strtok((*i), ","));
		routerNames.push_back(s);
		if(routeTable.find(s) == routeTable.end())
		{
			routeTable[s] = Entry(s, s, atoi(strtok(NULL, ",")));

			char * ip = strtok(strtok(NULL, ","), ":");
			Client *c = network__connect(net, ip, atoi(strtok(NULL, ":")));
			routeTable[s].setClient(c);
			routeTable[s].isNeighbor() = true;

			this->saction->link(c);
		}
		else
		{
			routeTable[s].dist() = atoi(strtok(NULL, ","));
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
	}
}

network* Router::getNetwork()
{
	return this->net;
}
