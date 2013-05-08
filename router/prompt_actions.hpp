#ifndef _NET_ACTIONS_HPP
#define _NET_ACTIONS_HPP

extern "C"
{
	#include "../common/messages.h"
	#include "../common/net.h"
}

#include "router.hpp"
#include "exec.hpp"

class PromptActions
{
	public:
		PromptActions(Router* r);
		void message(Message* m);
		void ping(Message* m, std::map<int, hdclock::time_point>& pingTimeTables);
		void route(Message* m);

	private:
		Router* router;
};
#endif
