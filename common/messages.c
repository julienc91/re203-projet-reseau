#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../trex/trex.h"
#include "messages.h"

char *str_sub (const char *s, unsigned int start, unsigned int end)
{
	char *new_s = NULL;

	if (s != NULL && start < end)
	{
		new_s = malloc (sizeof (*new_s) * (end - start + 2));
		if (new_s != NULL)
		{
			int i;

			for (i = start; i <= end; i++)
			{
				new_s[i-start] = s[i];
			}
			new_s[i-start] = '\0';
		}
		else
		{
			fprintf (stderr, "Memoire insuffisante\n");
			exit (EXIT_FAILURE);
		}
	}
	return new_s;
}

void mess__init(struct Message** mess)
{
	*mess = malloc(sizeof(struct Message));
	(*mess)->type = NONE;
	(*mess)->s_parameter = 0;
	(*mess)->node1 = 0;
	(*mess)->node2 = 0;
	(*mess)->weight = -1;
	(*mess)->ttl = -1;
	(*mess)->ok = 0;
}

void mess__parse(struct Message* mess_dest, char* mess_src, size_t len)
{
	mess__init(&mess_dest);

	static char* regex_strtable[] =
	{
		"load .*",
		"save .*",
		"show topology",
		"add link \\w* \\w* \\d*",
		"update link \\w* \\w* \\d*",
		"del link \\w* \\w*",
		"disconnect",
		"message \\w* \".*\"",
		"route \\w*",
		"routetable",
		"log in as \\w* port \\d*",
		"log in port \\d*",
		"logout",
		"greeting \\w*",
		"bye",
		"poll",
		"neighborhood newlist \\[.*\\]", //pourrait être amélioré pour vérifier directement si c'est bon
		"neighborhood ok",
		"link",
		"link ok",
		"vector \\[.*\\]",
		"vector ok",
		"packet src \\w* dst \\w* ttl \\d* .*",
		"packet src \\w* dst \\w* ok",
		"packet src \\w* dst \\w* toofar",
		"ping src \\w* dst \\w* ttl \\d*",
		"pong src \\w* dst \\w* ttl \\d*"
	};

	TRex* trex_table[30];
	int i;
	for(i = 0; i < 26; i++) // trouver le moyen de staticifier ça
	{
		trex_table[i] = trex_compile(regex_strtable[i], NULL);
		if(trex_match(trex_table[i], mess_src))
		{
			printf("\n\nLigne: %d\n", i);
			break;
		}
	}


	switch(i)
	{
		case 0:
			mess_dest->type = LOAD;
			mess_dest->s_parameter = str_sub(mess_src, 5, strlen(mess_src) - 1);
			break;
		case 1:
			mess_dest->type = SAVE;
			mess_dest->s_parameter = str_sub(mess_src, 5, strlen(mess_src) - 1);
			break;
		case 2:
			mess_dest->type = SHOW;
			break;
		case 3:
			mess_dest->type = ADDLINK;
			strtok(mess_src, " ");
			strtok(NULL, " ");
			mess_dest->node1 = strtok(NULL, " ");
			mess_dest->node2 = strtok(NULL, " ");
			mess_dest->n_parameter1 = atoi(strtok(NULL, " "));
			break;
		case 4:
			mess_dest->type = UPDATELINK;
			strtok(mess_src, " ");
			strtok(NULL, " ");
			mess_dest->node1 = strtok(NULL, " ");
			mess_dest->node2 = strtok(NULL, " ");
			mess_dest->n_parameter1 = atoi(strtok(NULL, " "));
			break;
		case 5:
			mess_dest->type = DELLINK;
			strtok(mess_src, " ");
			strtok(NULL, " ");
			mess_dest->node1 = strtok(NULL, " ");
			mess_dest->node2 = strtok(NULL, " ");
			break;
		case 6:
			mess_dest->type = DISCONNECT;
			break;
		case 7:
			mess_dest->type = MESSAGE;
			char * tmp;
			strcpy(tmp, mess_src);
			strtok(tmp, " ");

			mess_dest->node1 = strtok(NULL, " ");
			mess_dest->s_parameter = strstr(mess_src, mess_dest->node1) + strlen(mess_dest->node1) + 2;
			mess_dest->s_parameter[strlen(mess_dest->s_parameter) - 1] = 0;

			printf("N:%s\nMess:%s\n\n",  mess_dest->node1, mess_dest->s_parameter);
			break;
		case 8:
			mess_dest->type = ROUTE;
			strtok(mess_src, " ");
			mess_dest->node1 = strtok(NULL, " ");

			break;
		case 9:
			mess_dest->type = ROUTETABLE;
			break;
		case 10:
			mess_dest->type = LOGIN;
			//log in as ID port p
			strtok(mess_src, " ");
			strtok(NULL, " ");
			// cas as id...
			strtok(NULL, " ");
			mess_dest->node1 = strtok(NULL, " ");
			strtok(NULL, " ");
			mess_dest->n_parameter1 = atoi(strtok(NULL, " "));

			break;
		case 11:
			mess_dest->type = LOGIN;
			//log in port p
			strtok(mess_src, " ");
			strtok(NULL, " ");
			strtok(NULL, " ");
			mess_dest->n_parameter1 = atoi(strtok(NULL, " "));

			break;
		case 12:
			mess_dest->type = LOGOUT;
			break;
		case 13:
			mess_dest->type = GREETING;
			strtok(mess_src, " ");
			mess_dest->node1 = strtok(NULL, " ");
			break;
		case 14:
			mess_dest->type = BYE;
			break;
		case 15:
			mess_dest->type = POLL;
			break;
		case 16:
			mess_dest->type = NEIGHBORHOOD;
			break;
		case 17:
			mess_dest->type = NEIGHBORHOOD;
			break;
		case 18:
			mess_dest->type = LINK;
			break;
		case 19:
			mess_dest->type = LINK;
			break;
		case 20:
			mess_dest->type = VECTOR;
			break;
		case 21:
			mess_dest->type = VECTOR;
			break;
		case 22:
			mess_dest->type = PACKET;
			break;
		case 23:
			mess_dest->type = PACKET;
			break;
		case 24:
			mess_dest->type = PACKET;
			break;
		case 25:
			mess_dest->type = PING;
			break;
		case 26:
			mess_dest->type = PONG;
			break;
		default:
			break;
	}

}


int main(int argc, char * argv[])
{
	struct Message * m;
	//mess__parse(m, argv[1], 4);
	char * truc = malloc(sizeof(char)*100);
	strcpy(truc, "message n12 \"salut les copains!\"");
	mess__parse(m, truc, 4);
	return 0;
}
