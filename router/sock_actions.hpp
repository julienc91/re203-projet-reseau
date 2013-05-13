#ifndef _SOCK_ACTIONS_HPP
#define _SOCK_ACTIONS_HPP

extern "C"
{
	#include "../common/messages.h"
}
#include "router.hpp"

#include "../common/client.h"

class SockActions
{
	public:
		SockActions(Router* r);

		// messages génériques
		void reverse(Message* m);
		void forward(Message* m);

		// messages à envoyer au controller
		void login(int port, char* id = 0);
		void poll();
		void logout();

		// messages à envoyer à d'autres routeurs
		// (envoie à tous)
		void link(Client *t);
		void linkAck(Client* t);

		void vector(char* id, char * vect);


	private:
		Router* router;

};
#endif
