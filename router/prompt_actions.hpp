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
/**	\class PromptActions
 * 	\brief
 */
	public:
		PromptActions(Router* r);
		void message(Message* m, std::map<int, int>& seqnumTable);
		void ping(Message* m, std::map<int, hdclock::time_point>& pingTimeTables);
		void route(Message*, bool& waitingForRoute);

	private:
		Router* router;
};
#endif
