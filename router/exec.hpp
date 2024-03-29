#ifndef _EXEC_HPP
#define _EXEC_HPP

extern "C"
{
	#include "../common/messages.h"
	#include "../common/util.h"
	#include "../common/client.h"
    #include "../common/net.h"

}

#include <thread>
#include <map>

typedef std::chrono::steady_clock hdclock;
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

		void timeChecker();

	private:
		Router* router;
		Display* disp;

		bool isWaitingForRoute;
		char* routeDest;
		int routeCount;


		int pingCount; // pour savoir quand on reçoit un ping issu de la commande manuelle
		std::map<int, hdclock::time_point> pingTimeTables;
		int pingMin, pingMax, pingAvg, pingSucc;

		hdclock::time_point messageTime;
		hdclock::time_point routeTime;
		std::map<int, int> messageTimes; // clef: seqnum; val: temps

		std::thread *timeCheckerThread;
		bool threadRunning;
};

#endif
