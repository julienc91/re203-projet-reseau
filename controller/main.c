#include "graphlib.h"
#include "../common/prompt.h"
#include "exec.h"
#include "info_table.h"

extern network *net;

static void usage(char *bin)
{
    fprintf(stderr, "%s [<topology>]\n", bin);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc > 2) usage(argv[0]);
    
    graph__init();
    table__init();
    exec__init();
    
    if (argc == 2)
    {
        char *command = malloc(sizeof(char) * (strlen(argv[1]) + 7));
        strcpy(command, "load ");
        strcat(command, argv[1]);
        
        Message *m = mess__parse(command);
        exec__prompt_message(m);
        
        mess__free(&m);
        free(command);
    }

	printf("Graph prompt : Ctrl+D pour quitter\n");
	pthread_t *prompt_th1 = prompt__start(exec__prompt_message);
	
	// * * * * gestion serveur * * * *
	while(network__is_opened(net))
	{
        network__update(net);
	}
	fprintf(stderr, "exit main loop\n");

	// * * * * fermeture serveur * * * *
	
	network__close(net);
	network__free(net);

	// nettoyage de la table de hachage
	table__delete();
	graph__delete();
	
	return  0;
}
