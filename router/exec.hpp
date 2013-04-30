#ifndef _EXEC_HPP
#define _EXEC_HPP

extern "C"
{
	#include "../common/messages.h"
}

class PromptActions;
class Router;
class Display;

class Exec
{
	public:
		Exec(Router*);
		void prompt_message(Message* m);
		static void sock_message(Message* m);

	private:
		Router* router;
		PromptActions* action;
		Display* disp;
};

#endif
