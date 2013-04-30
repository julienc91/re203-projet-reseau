#include "display.h"
#include "graph_.h"
#include <unistd.h>
#include "exec.h"
#include "sock_table.h"
#include "net_functions.h"
#include "../common/util.h"

void exec__init(void)
{
	net = network__open(12345);
	//TODO ajouter le chargement du fichier de config 
	net->input_event = input_event;
	net->connection_event = connection_event;
	net->disconnection_event = disconnection_event;
	net->message_event = message_event;
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

	switch(m->type)
	{
		case LOAD:
			// actions sur graphe
			graph = graph__open(mess__unescape(m->s_parameter));
			//actions sur réseau
			//affichage
			if(graph != NULL)
			{
				disp__loaded(agnnodes(graph)); // calculer nombre de noeuds chargés
			}
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


struct Message *exec__sock_message(struct Message *m)
{
	Agnode_t *n1, *n2;
	Agedge_t *e;
	char voisinage[1000]="";
	char *id;
	char aux[100]="";
	Client_info *client_info;
	
	if(m == NULL)
	{
		return m;
	}

	switch(m->type)
	{
		case LOGIN:
			if(m->node1 == NULL) //cas ou on ne donne pas le noeud, en assigner un
			{
				n1 = agfstnode(graph);
				while(n1 != NULL && n1->u.is_connected == 1)
				{
					n1 = agnxtnode(graph, n1);
				}
				
				if(n1 == NULL)
				{
					printf("ERREUR : Aucun noeud libre\n"); 
					m->node1 = "";  
					return m;
				}
			}
			else //cas ou le noeud est donne, on verifie si il n'est pas deja utilise
			{
				n1 = agfindnode(graph, m->node1);
				if ((n1 == NULL) || (n1 != NULL && n1->u.is_connected == 1))
				{
					n1 = agfstnode(graph);
					while(n1 != NULL && n1->u.is_connected == 1)
					{
						n1 = agnxtnode(graph, n1);
					}
					
					if(n1 == NULL)
					{
						printf("ERREUR : Aucun noeud libre\n");   
						m->node1 = ""; 
						return m;	 
					}
				}
			}
			//ajouter les infos du client et l'id à la table
			strcopy2(&m->node1, graph__getId(n1));
			client_info = calloc(1,sizeof(Client_info));
			strcopy2(&client_info->address, m->s_parameter);
			client_info->port = m->n_parameter;
			table__add_info(graph__getId(n1), client_info);
			
			//actions sur le graphe
			n1->u.is_connected = 1;
			//envoyer greeting
			m->type = GREETING;
			break;

		case POLL:
			// si il y a eu un changement de voisinage, renvoyer la topologie
			agwrite(graph, stdout);
			
			n1 = agfindnode(graph, m->node1);

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
				
				if (n2->u.is_connected == 1)
				{
					id = graph__getId(n2);
					strcat(voisinage, id);
					strcat(voisinage, ",");
					sprintf(aux, "%d,", graph__getWeight(graph, e));
					strcat(voisinage, aux);
					
					aux[0]='\0';
					client_info = table__get_info(id);
					sprintf(aux, "%s", client_info->address);
					strcat(voisinage, aux);
					strcat(voisinage, ":");
					
					aux[0]='\0';
					sprintf(aux, "%d", client_info->port);
					strcat(voisinage, aux);
					strcat(voisinage, ";");
				}
				e = agnxtedge(graph, e, n1);
			}
			strcopy2(&m->s_parameter, voisinage);
			m->type = NEIGHBORHOOD;
			voisinage[0]='\0';
			aux[0]='\0';			
			// sinon, renvoyer neighborhood ok
			break;

		case LOGOUT:
			n1 = agfindnode(graph, m->node1);
			n1->u.is_connected = 0;
			m->type = BYE;
			table__delete_info(m->node1);
			// envoyer bye
			break;

		default:
			printf("ERREUR: Action non reconnue\n");
			break;
	}
	return m;
}
