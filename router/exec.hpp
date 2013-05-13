#ifndef _EXEC_HPP
#define _EXEC_HPP

extern "C"
{
	#include "../common/messages.h"
	#include "../common/util.h"
	#include "../common/client.h"
}


#include <chrono>
#include <map>

typedef std::chrono::high_resolution_clock hdclock;
typedef std::chrono::milliseconds milliseconds;


class Router;
class Display;

class Exec
{
	friend class Router;
	public:
		Exec(Router*);
		~Exec();
		void prompt_message(Message* m);
		void sock_message(Message* m, Client* t = 0);

	private:
		Router* router;
		Display* disp;

		bool isWaitingForRoute;
		char* routeDest;
		int routeCount;
		time_t routeTime;

		int pingCount; // pour savoir quand on reçoit un ping issu de la commande manuelle
		std::map<int, hdclock::time_point> pingTimeTables;
		int pingMin, pingMax, pingAvg, pingSucc;

		hdclock::time_point messageTime;
};

#endif
