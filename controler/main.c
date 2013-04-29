#include "graph_.h"
#include "../common/prompt.h"
#include "exec.h"
#include "sock_table.h"

extern network *net;

int main(void)
{
    graph__init();
    table__init();
    exec__init();

	//~ printf("Init ok: %p\n", net);
	printf("Graph prompt : Ctrl+D pour quitter\n");
	pthread_t *prompt_th1 = prompt__start(exec__prompt_message);
	

	//~ printf("Routeur prompt : Ctrl+D pour quitter\n");
	//~ pthread_t *prompt_th2 = prompt__start(exec__sock_message);
	//~ pthread_join(*prompt_th2, NULL);
	
	// * * * * gestion serveur * * * *
	while(network__is_opened(net))
	{
		network__update(net);
	}

	pthread_join(*prompt_th1, NULL);
	printf("Prompt quitté\n");

	// * * * * fermeture serveur * * * *
	
	network__close(net);
	network__free(net);

	return  0;
}
