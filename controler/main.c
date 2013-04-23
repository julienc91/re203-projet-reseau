#include "graph_.h"
#include "../common/prompt.h"
#include "exec.h"
#include "sock_table.h"

int main(void)
{
    graph__init();
    table__init();
    exec__init();
    
	//~ graph = graph__open("topo.dot");
	//~ agwrite(graph, stdout);

	//~ pthread_t *prompt_th = prompt__start(exec__prompt_message);
	pthread_t *prompt_th = prompt__start(exec__sock_message);

	pthread_join(*prompt_th, NULL);

	
	return  0;
}
