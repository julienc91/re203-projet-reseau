#include "graph_.h"
#include "../common/prompt.h"
#include "exec.h"
#include "sock_table.h"

int main(void)
{
    graph__init();
    table__init();
    exec__init();

	printf("Graph prompt : Ctrl+D pour quitter\n");
	pthread_t *prompt_th1 = prompt__start(exec__prompt_message);
	pthread_join(*prompt_th1, NULL);
	
	printf("Prompt quitté\n");

	//~ printf("Routeur prompt : Ctrl+D pour quitter\n");
	//~ pthread_t *prompt_th2 = prompt__start(exec__sock_message);
	//~ pthread_join(*prompt_th2, NULL);

	while(1)
	{
		network__update(net);
	}
	
	return  0;
}
