#include "router.hpp"

Router* glob__router = 0;

Router::Router(char* name, int srcport, int destport)
{
    _name = new std::string(name);
   // _tab = new RoutTable(name);     // pour l'instant ça plante

	exec = new Exec(this);

	init(); // windows compatibility

	// * * * * lecture fichier config * * * *
	config = config__readRouter();

	glob__router = this;

	// * * * * ouverture serveur * * * *
	net = network__open(srcport);



	// * * * * evenements * * * *
	net->input_event =  Event::input;
	net->connection_event = Event::connect;
	net->disconnection_event = Event::disconnect;
	net->message_event = Event::message;

	// * * * * connexion sortante * * * *
	printf("Connection to localhost on port %d...\n",destport);
	c = network__connect(net, "localhost", destport); //localhost à changer

	if (!c)
	{
		fprintf(stderr, "Connection failed.\n");
	}
	else
	{
		strcpy(c->id, "controlleur");
		printf("Connected on socket %d.\n", c->sock);
		char buf[100];
		sprintf(buf, "log in as %s", name);
		network__send(c, buf);
	}

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
