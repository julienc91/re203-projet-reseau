#include "../common/prompt.h"
#include "../common/messages.h"
#include "exec.h"

#include <pthread>

int main(void)
{
	pthread_t* prompt_th = prompt__start(exec__prompt_message);

/*
	char* origin = "j'aim\\e b\\ien\\* les\\\\ copains \\ 7 * 3*";
	char* bidule = mess__escape(origin);
	char* chose = mess__unescape(bidule);

	printf("\n\noriginal: %s\néchappé: %s\noriginal: %s\n", origin, bidule, chose);
*/
	pthread_join(*prompt_th, NULL);



}
