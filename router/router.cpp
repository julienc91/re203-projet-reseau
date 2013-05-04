#include "router.hpp"
#include "prompt_actions.hpp"
#include "sock_actions.hpp"
#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>

Router* glob__router = 0;

Router::Router(char* name, int srcport, int destport)
{
    _name = new std::string(name);
   // _tab = new RoutTable(name);     // pour l'instant ça plante

   	// * * * * lecture fichier config * * * *
	config = config__readRouter();

	exec = new Exec(this);
	paction = new PromptActions(this);
	saction = new SockActions(this);
	glob__router = this;

	init(); // windows compatibility



	// * * * * ouverture serveur * * * *
	net = network__open(srcport);

	// * * * * evenements * * * *
	net->input_event =  Event::input;
	net->connection_event = Event::connect;
	net->disconnection_event = Event::disconnect;
	net->message_event = Event::message;

	// * * * * connexion sortante * * * *
	std::cout << "Connection to localhost on port " << destport << std::endl;
	controller = network__connect(net, "localhost", destport); //localhost à changer

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

	parseNeighborhood((char*) "[a,b,c,d;e,f,g,h;ap,lol,kikoo]");

	// * * * * gestion stdin   * * * *
	void (Exec::*meth)(Message* m) = &Exec::prompt_message;
	prompt.start(exec, meth);

	// début
	mainLoop();
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

    delete _name;
    delete exec;
    delete saction;
    delete paction;
}

void Router::mainLoop()
{
	// * * * * gestion serveur * * * *
	while(network__is_opened(net))
	{
		network__update(net);
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
	for(i = v.begin(); i != v.end(); ++i)
	{
		r = strtok((*i), ",");
		routeTable[std::string(r)] = Entry(std::string(r), std::string(r), atoi(strtok(NULL, ",")));
		// TODO : set l'ip. Elle est avec le port (fmt : ip:port) dans le prochain strtok
		char * ip_src = strtok(NULL, ",");
		char * ip = strtok(ip_src, ":");
		routeTable[std::string(r)].setClient(network__connect(net, ip, atoi(strtok(NULL, ":"))));
	}

}
