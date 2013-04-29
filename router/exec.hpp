#ifndef _EXEC_HPP
#define _EXEC_HPP

#include "../common/net.h"
#include "../common/messages.h"

class Exec
{
	public:
		static void prompt_message(Message* m);
		static void sock_message(Message* m);
};

#endif
