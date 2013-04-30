#ifndef _PROMPT_HPP
#define _PROMPT_HPP
extern "C"
{
	#include "../common/messages.h"
}
#include <pthread.h>
#include "exec.hpp"

class Prompt
{
	public:
		~Prompt();

		void start(Exec* object, void (Exec::*mess_handler) (Message*));

	private:
		pthread_t* thread;
};
#endif
