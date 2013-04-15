#include <stdio.h>
#include "graph.h"
#include <graphviz/gvc.h>
#include <agraph.h>


Agraph_t* graph__open(char * filename){
    FILE * fd = fopen(filename, "r");
    Agraph_t * g = agread(fd);

    return g;
}


void graph__close(Agraph_t* graph){
    agclose(graph);
}

void graph__save(Agraph_t * graph, char * filename){



}

void graph__setWeight(Agraph_t* graph, Agedge_t* edge, int weight);
int graph__getWeight(Agraph_t* graph, Agedge_t* edge);

void graph__addEdge(Agraph_t* graph, Agnode_t* n1, Agnode_t* n2, int weight);
void graph__removeEdge(Agraph_t* graph, Agedge_t* edge);

