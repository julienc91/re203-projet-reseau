#include "prompt.hpp"


#include <cstring>
#include <cstdio>
struct ThreadData
{
	void (Exec::*mess_handler) (Message*);
	Exec *object;
};

void* main_thread(void* v);

Prompt::~Prompt()
{
	pthread_join(*thread, NULL);
}

void Prompt::start(Exec* object, void (Exec::*mess_handler) (Message*))
{
	thread = new pthread_t;
	ThreadData *dat = new ThreadData;
	dat->mess_handler = mess_handler;
	dat->object = object;

	pthread_create(thread, NULL, main_thread, (void*) dat);
}

void* main_thread(void* v)
{
	ThreadData *dat = (ThreadData*) v;
	void (Exec::*handler) (Message*) = dat->mess_handler;
	char * s = new char[256];
	struct Message* m = NULL;
	while((s = fgets(s, 255, stdin)) != NULL)
	{
		s[strlen(s) - 1] = 0;
		m = mess__parse(s);

		//mess__debug(m);
		if(v != NULL && m != NULL)
		{
			(dat->object->*handler)(m);
		}
	}

	return NULL;
}
