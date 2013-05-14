/**
 * \file graphlib.c
 * \brief A graph API
 * 
 * Manages a graph using the graphviz library.
 * 
 */

#include <stdio.h>
#include <string.h>
#include "graphlib.h"
#include "../common/util.h"

#define TMP_FILENAME "graph_tmp.dot"
#define MAX_SIZE 512

/**
 *  \brief Initialization
 *
 */
void graph__init(void)
{
     aginit();
}

/**
 *  \brief Open a graph from a dot file and create a graph structure
 *  \param filename The path to the dot file to load.
 *  \return A pointer to the created graph structure.
 */
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

/**
 *  \brief Delete the graph structure
 *
 */
void graph__delete(){
     graph__close(graph);
}

/**
 *  \brief Close the graph structure
 *
 */
void graph__close(Agraph_t* graph){
     agclose(graph);
}

/**
 *  \brief Save the graph structure into a file
 *  \param graph The graph to save.
 *  \param filename The output file.
 *  \return The result of the agwrite call.
 *
 */
int graph__save(Agraph_t * graph, char * filename){
     FILE * f = fopen(filename, "w");
     int i = agwrite(graph, f);
     fclose(f);
     return i;
}

/**
 *  \brief Show a graph on the prompt
 *  \param graph The graph to show.
 *
 */
void graph__show(Agraph_t *graph) {
     FILE* file;
     if((file = fopen(TMP_FILENAME, "r")) != NULL) {
       fclose(file);
       fprintf(stderr, "Error: %s file already exists\n", TMP_FILENAME);
       return;
     }
     file = fopen(TMP_FILENAME, "w+");
     agwrite(graph, file);

     char buffer[MAX_SIZE] = "";
     rewind(file);
     fgets(buffer, MAX_SIZE, file);
     while(fgets(buffer, MAX_SIZE, file) != NULL) {
	  if(strncmp(buffer, "}", 1) != 0)
	       printf("%s", buffer);
     }
     fclose(file);
     remove(TMP_FILENAME);
}

/**
 *  \brief Set a weight to an edge
 *  \param graph The graph to modify.
 *  \param edge The edge whose weight must be changed.
 *  \param weight The new weight of the edge.
 *
 */
void graph__setWeight(Agraph_t* graph, Agedge_t* edge, int weight){
     char aux[10];
     sprintf(aux, "%d", weight); 
     agset(edge, "label",  aux);
     edge->head->u.is_up_to_date = 0;
     edge->tail->u.is_up_to_date = 0;
}

/**
 *  \brief Get an edge's weight
 *  \param graph The graph to work on.
 *  \param edge The edge whose weight is wanted.
 *  \return The edge's weight.
 *
 */
int graph__getWeight(Agraph_t* graph, Agedge_t* edge){
     return 	atoi(agget(edge, "label"));
}

/**
 *  \brief Add an edge to a graph
 *  \param graph The graph to modify.
 *  \param n1 The first node of the new edge.
 *  \param n2 The second node of the new edge.
 *  \param weight The weight of the new edge.
 *
 */
void graph__addEdge(Agraph_t* graph, Agnode_t* n1, Agnode_t* n2, int weight){
     Agedge_t *e = agedge(graph, n1, n2);
     char aux[10];
     sprintf(aux, "%d", weight); 
     agset(e, "label",  aux);
}

/**
 *  \brief Remove an edge from a graph
 *  \param graph The graph to modify.
 *  \param edge The edge to remove.
 *
 */
void graph__removeEdge(Agraph_t* graph, Agedge_t* edge){
     edge->head->u.is_up_to_date = 0;
     edge->tail->u.is_up_to_date = 0;
     agdelete(graph, edge);
}

/**
 *  \brief Get a node's id
 *  \param node A node.
 *  \return The node's id.
 *
 */
char* graph__getId(Agnode_t* node)
{
     return node->name;
     //~ return agget(node, "label");
}
