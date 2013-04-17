#include <stdio.h>
#include <graphviz/types.h>
#include <graphviz/graph.h>
#include "graph_.h"

Agraph_t* graph__open(char * filename){
    FILE * fd = fopen(filename, "r");
    Agraph_t * g = agread(fd);
    fclose(fd);

    return g;
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
    edge->u->weight = weight;
}

int graph__getWeight(Agraph_t* graph, Agedge_t* edge){
    return edge->u->weight;
}


void graph__addEdge(Agraph_t* graph, Agnode_t* n1, Agnode_t* n2, int weight){
    Aedge_t *e = agedge(graph, n1, n2);
    e->u->weight = weight;
}

int graph__removeEdge(Agraph_t* graph, Agedge_t* edge){
    return agdelete(graph, (void *)edge);
}

