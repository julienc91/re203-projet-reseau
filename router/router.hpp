#ifndef _ROUTER_HPP
#define _ROUTER_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>

extern "C" {
#include "../common/net.h"
#include "../common/messages.h"
#include "../common/prompt.h"
#include "../common/config.h"
}

#include "entry.hpp"
#include "prompt.hpp"

class Exec;
#include "event.hpp"
#include "routetable.hpp"
class Router
{
	friend class Event;
	public:
		Router(char* name, int srcport, int destport);
		Router(const Router * data);
		~Router();

		void mainLoop();

		int newSeqnum();
		char* getName();
		void setName(char*);
		Configuration* getConfiguration();

		RouteTable& getRouteTable();



	private:
		std::string *_name;
		RouteTable routeTable;

		network* net;
		Client* c;

		Configuration* config;
		int seqnum;
		Prompt prompt;
		Exec *exec;
};
#endif
