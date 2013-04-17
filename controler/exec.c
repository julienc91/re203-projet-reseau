#include "display.h"
void exec__prompt_message(struct Message* m)
{
	if(m == NULL)
	{
		return 0;
	}

	switch(m->type)
	{
		case LOAD:
			disp__loaded(0); // calculer nombre de noeuds chargés
			break;

		case SAVE:
			disp__saved(0); // calculer nombre de noeuds sauvés

		case SHOW:
			disp__topology();
			break;

		case ADDLINK:
			// vérifier que l'on peut ajouter ce lien
			disp__addlink(m->node1, m->node2, mess__getWeight(m));
			break;
		case UPDATELINK:
			// vérifier que l'on peut ajouter ce lien
			disp__updlink(m->node1, m->node2, mess__getWeight(m));
			break;
		case DELLINK:
			// vérifier que l'on peut ajouter ce lien
			disp__dellink(m->node1, m->node2);
			break;
		case DISCONNECT:
			// vérifier que l'on peut ajouter ce lien
			disp__disconnect(m->node1);
			break;
	}
}
