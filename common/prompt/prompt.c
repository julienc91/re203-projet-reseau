#include "prompt.h"
#include "../messages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_t* promp__start(void)
{
	pthread_t* prompt_thread = malloc(sizeof(pthread_t));
	pthread_create(prompt_thread, NULL, prompt__main_thread, NULL);

	return prompt_thread;
}

void* prompt__main_thread(void* v)
{
	v = v; // pour warnings
	char * s = malloc(256);
	struct Message* m;
	while((s = fgets(s, 255, stdin)) != NULL)
	{
		s[strlen(s) - 1] = 0;
		m = mess__parse(s);
		mess__debug(m);
	}

	return NULL;
}

int main(void)
{
	pthread_t* prompt_th = promp__start();
	pthread_join(*prompt_th, NULL);
}
