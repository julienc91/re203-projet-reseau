#include "display.h"
#include "graph_.h"
#include <unistd.h>
#include "exec.h"
#include "sock_table.h"

void exec__init(void)
{
	net = network__open(12345);
	//ajouter le chargement du fichier de config 
}

void exec__prompt_message(struct Message *m)
{
	if(m == NULL)
	{
		return;
	}

	Agnode_t *n1, *n2;
	Agedge_t *e;
	struct Message reponse;
	//~ reponse.n_parameter = NULL;

	switch(m->type)
	{
		case LOAD:
			// actions sur graphe
			graph = graph__open(mess__unescape(m->s_parameter));
			//actions sur réseau
			//affichage
			disp__loaded(agnnodes(graph)); // calculer nombre de noeuds chargés
			break;

		case SAVE:
			// actions sur graphe
			graph__save(graph, mess__unescape(m->s_parameter));
			//actions sur réseau
			/*NONE*/
			//affichage
			disp__saved(agnnodes(graph)); // calculer nombre de noeuds sauvés
			break;

		case SHOW:
			// actions sur graphe
			/*NONE*/
			//actions sur réseau
			/*NONE*/
			//affichage
			agwrite(graph, stdout);
			//disp__topology();
			break;

		case ADDLINK:
			// actions sur graphe
			graph__addEdge(graph, agfindnode(graph, m->node1), agfindnode(graph, m->node2), mess__getWeight(m));
			//actions sur réseau

			//affichage
			// vérifier que l'on peut ajouter ce lien
			disp__addlink(m->node1, m->node2, mess__getWeight(m));
			break;

		case UPDATELINK:
			// actions sur graphe
			graph__setWeight(graph, agfindedge(graph,agfindnode(graph, m->node1), agfindnode(graph, m->node2)) , mess__getWeight(m));
			//actions sur réseau

			//affichage
			// vérifier que l'on peut modifier ce lien
			disp__updlink(m->node1, m->node2, mess__getWeight(m));
			break;

		case DELLINK:
			// actions sur graphe
			graph__removeEdge(graph, agfindedge(graph,agfindnode(graph, m->node1), agfindnode(graph, m->node2)));
			//actions sur réseau

			//affichage
			// vérifier que l'on peut supprimer ce lien
			disp__dellink(m->node1, m->node2);
			break;

		case DISCONNECT:
			// actions sur graphe
			n1 = agfindnode(graph, m->node1);
			e = agfstedge(graph, n1);
			while(e!=NULL)
			{
				agdelete(graph,e);
				e = agfstedge(graph, n1);
			}
			//actions sur réseau

			//affichage
			// vérifier que l'on peut déconnecter ce noeud
			disp__disconnect(m->node1);
			break;

		default:
			printf("ERREUR: Action non reconnue\n");
			break;
	}
}


void exec__sock_message(struct Message *m)
{
	Agnode_t *n1, *n2;
	Agedge_t *e;
	char voisinage[1000]="";
	char *id;
	char aux[100]="";
	Client * client;
	
	if(m == NULL)
	{
		return;
	}

	switch(m->type)
	{
		case LOGIN:
			if(m->node1 == NULL) //cas ou on ne donne pas le noeud, en assigner un
			{
				
			}
			else //cas ou le noeud est donne, on verifie si il n'est pas deja utilise
			{
				
			}
			//actions sur le graphe
			/*NONE*/
			//envoyer greeting
			break;

		case POLL:
			// si il y a eu un changement de voisinage, renvoyer la topologie
			//~ n1 = agfindnode(graph, m->node1);
			//~ fprintf(stderr, "ICI \n");
			graph = graph__open("topo.dot");

			agwrite(graph, stdout);
			//~ fprintf(stderr, "LA\n");

			n1 = agfstnode(graph);

			e = agfstedge(graph, n1);
			while(e!=NULL)
			{
				if(e->tail != n1)
				{
					n2 = e->tail;
				}
				else
				{
					n2 = e->head;
				}
				
				id = agget(n2, "label");
				strcat(voisinage, id);
				strcat(voisinage, ",");
				sprintf(aux, "%d,", graph__getWeight(graph, e));
				strcat(voisinage, aux);
				//TODO ajouter l ip
				aux[0]='\0';
				client = table__get_socket(id);
				sprintf(aux, client__get_address(client));
				strcat(voisinage, aux);
				strcat(voisinage, ":");
				//TODO ajouter le port
				aux[0]='\0';
				sprintf(aux, client__get_port(client));
				strcat(voisinage, aux);
				strcat(voisinage, ";");
				
				e = agnxtedge(graph, e, n1);
			}
			printf("le voisinage : %s\n",voisinage);
			voisinage[0]='\0';
			aux[0]='\0';			
			// sinon, renvoyer neighborhood ok
			break;

		case LOGOUT:
			// envoyer bye
			break;

		default:
			printf("ERREUR: Action non reconnue\n");
			break;
	}
}
