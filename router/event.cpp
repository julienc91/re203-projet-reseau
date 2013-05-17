#include <iostream>
#include <cstring>

extern "C"
{
#include "../common/net.h"
#include "../common/messages.h"
}
#include "event.hpp"
#include "exec.hpp"

extern Router* glob__router;

void Event::input(network *net, char *buffer)
{
  std::cout << "<input event: '" << buffer << "'>" << std::endl;

  if (strcmp(buffer, "quit") == 0)
    {
      network__close(net);
      return;
    }

  network__send(&(net->clients[0]), buffer);
}

void Event::connect(network *net, Client *c, char *buffer)
{

  Messages *m = mess__multiline_parse(buffer);

  if (!m) return;

  for (unsigned int i = 0; i < m->nb_messages; i++)
    {
      if (m->messages[i] != NULL)
     	{
#ifdef __ROUTER_DEBUG
            char *tmp = mess__toString(m->messages[i]);
            std::cout << "<connection from '" << c->id
                    << "' on socket '" << c->sock << "' : '"
                    << tmp
                    << "'>" << std::endl;
            free(tmp);
#endif
            glob__router->exec->sock_message(m->messages[i], c);
        }
    }
  mess__free_messages(&m);
}

void Event::disconnect(network *net, Client *c)
{
  std::cerr << "<disconnection from socket '" << c->sock << "' of client '"<< c->id <<"'>" << std::endl;
  if (strcmp(c->id, "controlleur") == 0)
  {
    network__close(net);
    network__free(net);
    std::cerr << "[ROUTER] Closed by controller." << std::endl;
    exit(0);
  }
  //~ else
  //~ {
	//~ if(glob__router->getRouteTable().find(c->id) != glob__router->getRouteTable().end())
	//~ {
		//~ glob__router->getRouteTable()[c->id].setClient(0);
	//~ }
  //~ }
}

void Event::message(network *net, Client *c, char *buffer)
{
  Messages *m = mess__multiline_parse(buffer);

  if (!m) return;

  for (unsigned int i = 0; i < m->nb_messages; i++)
    {
      if (m->messages[i] != NULL)
	{
#ifdef __ROUTER_DEBUG
	  char *tmp = mess__toString(m->messages[i]);
	   std::cout << "<message from '" << c->id
		     << "' on socket '" << c->sock << "' : '"
		     << tmp
		     << "'>" << std::endl;
	  free(tmp);
#endif
	  glob__router->exec->sock_message(m->messages[i], c);
	}
    }
  mess__free_messages(&m);
}
