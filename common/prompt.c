#include "prompt.h"
#include "messages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
pthread_t* prompt__start(int (*mess_handler) (void*, struct Message*))
#else
pthread_t* prompt__start(int (*mess_handler) (struct Message*))
#endif
{
	pthread_t* prompt_thread = malloc(sizeof(pthread_t));
	pthread_create(prompt_thread, NULL, prompt__main_thread, (void*)mess_handler);

	return prompt_thread;
}

void* prompt__main_thread(void* v)
{
	char * s = malloc(256);
	struct Message* m = NULL;
	while((s = fgets(s, 255, stdin)) != NULL)
	{
		s[strlen(s) - 1] = 0;
		m = mess__parse(s);

		//mess__debug(m);
		if(v != NULL && m != NULL)
		{
			#ifdef __cplusplus
			((void (*)(void*, struct Message*)) v)(m);
			#else
			((void (*)(struct Message*)) v)(m);
			#endif
		}
	}

	return NULL;
}
