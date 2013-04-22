#include "graph_.h"
#include "../common/prompt.h"
#include "exec.h"
#include "sock_table.h"
#include "../src/net.h"

int main(void)
{
    graph__init();
    init_table();
   
	pthread_t *prompt_th = prompt__start(exec__prompt_message);
	pthread_join(*prompt_th, NULL);


	network_close(net);
	network_free(net);
	
	return  0;
}
