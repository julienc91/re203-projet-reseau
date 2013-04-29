#ifndef _NET_ACTIONS_HPP
#define _NET_ACTIONS_HPP

extern "C"
{
	#include "../common/messages.h"
	#include "../common/net.h"
}

class Router;
class NetActions
{
	public:
		void message(Message* m);
		void ping(Message* m);

	private:
		Router* router;
};
#endif
