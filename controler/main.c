//~ #include <graphviz/types.h>
//~ #include <graphviz/graph.h>
#include "graph_.h"


#include "../common/prompt.h"
#include "../common/messages.h"
#include "exec.h"

#include <stdio.h>
#include <unistd.h>

int main(void)
{
	//~ Agraph_t *graph;
    aginit();
    
    /*
	Agraph_t *graph = graph__open("topo.dot");
	
	Agnode_t *n1, *n2, *n5, *n6;
	Agedge_t *n3;

	n1 = agfindnode(graph, "n1");		
	printf("n1\n");	
	n2 = agfindnode(graph, "n4");
	printf("n2\n");		
	graph__addEdge(graph, n1, n2, 2);
	n3 = agfindedge(graph, n1, n2);
	printf("n3 : %d\n", n3->u.weight);	
	
	agwrite(graph, stdout);

	n5 = agnode(graph, "n5");
	n6 = agfindnode(graph, "n5");
	printf("test : %d\n", (n5==n6));
	*/



	pthread_t *prompt_th = prompt__start(exec__prompt_message);

/*
	char* origin = "j'aim\\e b\\ien\\* les\\\\ copains \\ 7 * 3*";
	char* bidule = mess__escape(origin);
	char* chose = mess__unescape(bidule);

	printf("\n\noriginal: %s\néchappé: %s\noriginal: %s\n", origin, bidule, chose);
*/
	pthread_join(*prompt_th, NULL);



}
