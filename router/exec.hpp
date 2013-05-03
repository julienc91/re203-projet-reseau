#ifndef _EXEC_HPP
#define _EXEC_HPP

extern "C"
{
	#include "../common/messages.h"
	#include "../common/util.h"
}


class Router;
class Display;

class Exec
{
	public:
		Exec(Router*);
		void prompt_message(Message* m);
		void sock_message(Message* m);

	private:
		Router* router;
		Display* disp;

		bool isWaitingForRoute;
		char* routeDest;
		int routeCount;
		time_t routeTime;

		int pingCount; // pour savoir quand on reçoit un ping issu de la commande manuelle
		time_t * pingTimeTables;
};

#endif
