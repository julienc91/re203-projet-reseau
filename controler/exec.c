#include "display.h"

void exec__prompt_message(struct Message* m)
{
	if(m == NULL)
	{
		return;
	}

	switch(m->type)
	{
		case LOAD:
			// actions sur graphe

			//actions sur réseau

			//affichage
			disp__loaded(0); // calculer nombre de noeuds chargés
			break;

		case SAVE:
			// actions sur graphe

			//actions sur réseau

			//affichage
			disp__saved(0); // calculer nombre de noeuds sauvés
			break;

		case SHOW:
			// actions sur graphe

			//actions sur réseau

			//affichage
			disp__topology();
			break;

		case ADDLINK:
			// actions sur graphe

			//actions sur réseau

			//affichage
			// vérifier que l'on peut ajouter ce lien
			disp__addlink(m->node1, m->node2, mess__getWeight(m));
			break;

		case UPDATELINK:
			// actions sur graphe

			//actions sur réseau

			//affichage
			// vérifier que l'on peut modifier ce lien
			disp__updlink(m->node1, m->node2, mess__getWeight(m));
			break;

		case DELLINK:
			// actions sur graphe

			//actions sur réseau

			//affichage
			// vérifier que l'on peut supprimer ce lien
			disp__dellink(m->node1, m->node2);
			break;

		case DISCONNECT:
			// actions sur graphe

			//actions sur réseau

			//affichage
			// vérifier que l'on peut déconnecter ce noeud
			disp__disconnect(m->node1);
			break;

		default:
			printf("ERREUR: Action non reconnue");
			break;
	}
}


void exec__sock_message(struct Message* m)
{
	if(m == NULL)
	{
		return;
	}

	switch(m->type)
	{
		case LOGIN:
			if(m->node1 != NULL) //cas ou on ne donne pas le noeud, en assigner un
			{

			}

			//actions sur le graphe

			//envoyer greeting
			break;

		case POLL:
			// si il y a eu un changement de voisinnage, renvoyer la topologie

			// sinon, renvoyer neighborhood ok
			break;

		case LOGOUT:
			// envoyer bye
			break;

		default:
			printf("ERREUR: Action non reconnue");
			break;
	}
}
