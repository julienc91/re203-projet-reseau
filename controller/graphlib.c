#include <stdio.h>
#include "graphlib.h"
#include "../common/util.h"

void graph__init(void)
{
	aginit();
}

Agraph_t* graph__open(char * filename){
    FILE * fd = fopen(filename, "r");
    if(fd == NULL){
        perror("fopen");
        return NULL;
    }

    Agraph_t *g = agread(fd);
    fclose(fd);

    return g;
}

void graph__delete(){
    graph__close(graph);
}

void graph__close(Agraph_t* graph){
    agclose(graph);
}

int graph__save(Agraph_t * graph, char * filename){
    FILE * f = fopen(filename, "w");
    int i = agwrite(graph, f);
    fclose(f);
    return i;
}

void graph__setWeight(Agraph_t* graph, Agedge_t* edge, int weight){
    char aux[10];
    sprintf(aux, "%d", weight); 
	agset(edge, "label",  aux);
	edge->head->u.is_up_to_date = 0;
	edge->tail->u.is_up_to_date = 0;
}

int graph__getWeight(Agraph_t* graph, Agedge_t* edge){
	return 	atoi(agget(edge, "label"));
}

void graph__addEdge(Agraph_t* graph, Agnode_t* n1, Agnode_t* n2, int weight){
    Agedge_t *e = agedge(graph, n1, n2);
    char aux[10];
    sprintf(aux, "%d", weight); 
	agset(e, "label",  aux);
}

void graph__removeEdge(Agraph_t* graph, Agedge_t* edge){
	edge->head->u.is_up_to_date = 0;
	edge->tail->u.is_up_to_date = 0;
    agdelete(graph, edge);
}

char* graph__getId(Agnode_t* node)
{
	return node->name;
	//~ return agget(node, "label");
}
