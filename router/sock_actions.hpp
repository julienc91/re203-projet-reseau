#ifndef _SOCK_ACTIONS_HPP
#define _SOCK_ACTIONS_HPP

extern "C"
{
	#include "../common/messages.h"
}
#include "router.hpp"

class SockActions
{
	public:
		SockActions(Router* r);
		void reverse(Message* m);
		void forward(Message* m);

	private:
		Router* router;

};
#endif
