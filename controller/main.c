#include "graphlib.h"
#include "../common/prompt.h"
#include "exec.h"
#include "info_table.h"

extern network *net;

void *network_loop(void *net)
{
    // * * * * gestion serveur * * * *
	while(network__is_opened((network *)net))
	{
        network__update((network *)net);
	}
}

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
	
    pthread_t network_thread;
    pthread_create(&network_thread, NULL, network_loop, (void *)net);
    
    pthread_join(*prompt_th1, NULL);
    
    pthread_cancel(network_thread);
    
    pthread_join(network_thread, NULL);

    printf("[CONTROLLER] Closing network...\n");

	// * * * * fermeture serveur * * * *
	network__close(net);
	network__free(net);

	// Free memory
	table__delete();
	graph__delete();
    free (prompt_th1);
    free (config);
    mess__base__free();
    
    agclose(graph);
	
    printf("[CONTROLLER] Closed.\n");
    
	return  0;
}
