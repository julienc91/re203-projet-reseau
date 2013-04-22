#include "prompt.h"
#include "messages.h"
#include "config.h"

#include <stdio.h>
int main(void)
{
	config__readController();
	/*
	pthread_t* prompt_th = prompt__start(NULL);


	char* origin = "j'aim\\e b\\ien\\* les\\\\ copains \\ 7 * 3*";
	char* bidule = mess__escape(origin);
	char* chose = mess__unescape(bidule);

	printf("\n\noriginal: %s\néchappé: %s\noriginal: %s\n", origin, bidule, chose);
	pthread_join(*prompt_th, NULL);*/

}
