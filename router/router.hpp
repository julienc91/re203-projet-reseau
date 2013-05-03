#ifndef _ROUTER_HPP
#define _ROUTER_HPP

extern "C" {
#include "../common/net.h"
#include "../common/messages.h"
#include "../common/prompt.h"
#include "../common/config.h"
}

#include "entry.hpp"
#include "prompt.hpp"
#include "event.hpp"
#include "routetable.hpp"

class Exec;
class PromptActions;
class SockActions;

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
		Client* getController();

		PromptActions* promptActions();
		SockActions* sockActions();

	private:
		std::string *_name;
		RouteTable routeTable;

		PromptActions* paction;
		SockActions* saction;

		network* net;
		Client* controller;

		Configuration* config;
		int seqnum;
		Prompt prompt;
		Exec *exec;
};
#endif
