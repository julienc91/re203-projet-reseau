#include "graphlib.h"
#include "../common/prompt.h"
#include "exec.h"
#include "info_table.h"

extern network *net;

int main(void)
{
    graph__init();
    table__init();
    exec__init();

	printf("Graph prompt : Ctrl+D pour quitter\n");
	pthread_t *prompt_th1 = prompt__start(exec__prompt_message);
	
	// * * * * gestion serveur * * * *
	while(network__is_opened(net))
	{
		network__update(net);
	}

	//~ printf("Prompt quitté\n");

	// * * * * fermeture serveur * * * *
	
	network__close(net);
	network__free(net);

	// nettoyage de la table de hachage
	table__delete();
	graph__delete();
	
	return  0;
}
