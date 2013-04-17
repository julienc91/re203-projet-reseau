#include "prompt.h"
#include "messages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_t* prompt__start(void (*mess_handler) (struct Message*))
{
	pthread_t* prompt_thread = malloc(sizeof(pthread_t));
	pthread_create(prompt_thread, NULL, prompt__main_thread, (void*)mess_handler);

	return prompt_thread;
}

void* prompt__main_thread(void* v)
{
	char * s = malloc(256);
	struct Message* m;
	while((s = fgets(s, 255, stdin)) != NULL)
	{
		s[strlen(s) - 1] = 0;
		m = mess__parse(s);

		mess__debug(m);
		if(v != NULL)
		{
			((void (*)(struct Message*)) v)(m); //plus belle ligne de code que j'ai jamais écrite
		}
	}

	return NULL;
}
