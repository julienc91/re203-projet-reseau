#ifndef __GRAPH_H_
#define __GRAPH_H_

typedef struct{
	} Agraphinfo_t;
typedef struct{
	 int is_connected;
	 int is_up_to_date;
	} Agnodeinfo_t;
typedef struct{
	} Agedgeinfo_t;

/* IMPORTANT : DEFINIR LES STRUCTURES AVANT L INCLUDE SUIVANT */
#include <graphviz/graph.h>

static Agraph_t *graph = NULL;

void graph__init(void);
void graph__delete(void);

Agraph_t* graph__open(char* filename);
void graph__close(Agraph_t* graph);

int graph__save(Agraph_t* graph, char* filename);
void graph__show(Agraph_t *graph);

void graph__setWeight(Agraph_t* graph, Agedge_t* edge, int weight);
int graph__getWeight(Agraph_t* graph, Agedge_t* edge);

void graph__addEdge(Agraph_t* graph, Agnode_t* n1, Agnode_t* n2, int weight);
void graph__removeEdge(Agraph_t* graph, Agedge_t* edge);

char* graph__getId(Agnode_t* node);

#endif
