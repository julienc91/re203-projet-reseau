#include "prompt.hpp"

#include <cstring>
#include <cstdio>
struct ThreadData
{
/** \struct ThreadData
 */
	void (Exec::*mess_handler) (Message*);
	Exec *object;
};

void* main_thread(void* v);

/**	\brief initialize the Propmt instance
 */
Prompt::Prompt() :
	isRunning(true)
{

}

/**	\brief Set the prompt to non running state
 */
void Prompt::stop()
{
	isRunning = false;
}

/**	\brief destoy the instance
 * 
 */
Prompt::~Prompt()
{
	thread->join();
	delete thread;
}

/** \brief Start the prompt
 *  \param 
 */
void Prompt::start(Exec* object, void (Exec::*mess_handler) (Message*))
{
	ThreadData *dat = new ThreadData;
	dat->mess_handler = mess_handler;
	dat->object = object;

	thread = new std::thread(&Prompt::main_thread, this, (void*) dat);
}
/** \brief
 * 	\param v
 */
void Prompt::main_thread(void* v)
{
	ThreadData *dat = (ThreadData*) v;
	void (Exec::*handler) (Message*) = dat->mess_handler;
	char * s = new char[256];
	struct Message* m = 0;
	while((s = fgets(s, 255, stdin)) != 0 && isRunning)
	{
		s[strlen(s) - 1] = 0;
		m = mess__parse(s);

		//mess__debug(m);
		if(v != 0 && m != 0)
		{
			(dat->object->*handler)(m);
		}
	}

	delete dat;
}
