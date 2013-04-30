#include "router.hpp"

Routeur::Routeur(char* name, int srcport, int destport)
{
    _name = new std::string(name);
   // _tab = new RoutTable(name);     // pour l'instant ça plante

	init(); // windows compatibility

	// * * * * lecture fichier config * * * *
	config = config__readRouter();

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
	prompt.start(&exec, meth);


	// début
	mainLoop();

}

Routeur::Routeur(const Routeur * data)
{
    _name   = data->_name;
    _tab    = new RoutTable(data->_tab);
}

Routeur::~Routeur()
{
	// * * * * fermeture serveur * * * *

	network__close(net);
	network__free(net);

	end(); // windows compatibility

    delete _tab;
    delete _name;
}

void Routeur::mainLoop()
{
	// * * * * gestion serveur * * * *
	while(network__is_opened(net))
	{
		network__update(net);
	}
}

int Routeur::newSeqnum()
{
	return ++seqnum;
}
char* Routeur::getName()
{
	return (char*) _name->c_str();
}

Configuration* Routeur::getConfiguration()
{
	return config;
}
