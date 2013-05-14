#ifndef _PROMPT_HPP
#define _PROMPT_HPP
extern "C"
{
	#include "../common/messages.h"
}
#include <thread>
#include "exec.hpp"

class Prompt
{
/**	\class Prompt
 *  \brief manage the prompt
 */
 
	public:
		Prompt();
		~Prompt();

		void start(Exec* object, void (Exec::*mess_handler) (Message*));
		void main_thread(void* v);

		void stop();

	private:
		std::thread* thread;
		bool isRunning;
};
#endif
