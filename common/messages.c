#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../trex/trex.h"
#include "messages.h"

char *str_sub (const char *s, unsigned int start, unsigned int end);

char* strcopy(const char* src);

void mess__init(struct Message** mess)
{
	*mess = malloc(sizeof(struct Message));
	(*mess)->type = NONE;
	(*mess)->s_parameter = NULL;
	(*mess)->node1 = NULL;
	(*mess)->node2 = NULL;
	(*mess)->n_parameter = -1;
	(*mess)->accept = NOT;
}

int mess__getWeight(struct Message* mess)
{
	return mess->n_parameter;
}

int mess__getTTL(struct Message* mess)
{
	return mess->n_parameter;
}


int mess__getAcceptance(struct Message* mess)
{
	return mess->accept;
}
/*
 *
 * A utiliser lors de l'envoi, première chose à faire dans autre sens : appeller unescape.
 */
char* mess__escape(char* mess_src)
{
	int count = 0, i;
	for(i = 0; mess_src[i] != 0; i++)
	{
		if(mess_src[i] == '*' || mess_src[i] == '\\')
		{
			count++;
		}
	}
	int n = i; // taille de mess_src moins un caractère

	char* mess_escp = malloc((n + 1 + count) * sizeof(char));

	count = 0;
	for(int i = 0; i <= n; i++)
	{
		if((mess_src[i] == '*' || mess_src[i] == '\\') && i != n-1)
		{
			mess_escp[count++] = '\\';
			mess_escp[count] = mess_src[i];
		}
		else
		{
			mess_escp[count] = mess_src[i];
		}
		++count;
	}

	return mess_escp;
}

/** à utiliser lors de la réception*/
char* mess__unescape(char* mess_src)
{
	int count = 0, n = strlen(mess_src);
	char* mess_unescp = malloc(n * sizeof(char)); // TODO: à affiner

	for(int i = 0; i <= n; i++)
	{
		if((mess_src[i] == '\\' && i+1 <= n && mess_src[i+1] == '\\') ||
			(mess_src[i] == '\\' && i+1 <= n && mess_src[i+1] == '*') )
		{
			++i;
		}

		mess_unescp[count] = mess_src[i];
		++count;
	}

	return mess_unescp;
}
// fait l'allocation de mess_dest
struct Message* mess__parse(char* mess_src)
{
	struct Message* mess_dest;
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
		"pong src \\w* dst \\w* ttl \\d*",
		"ping \\w*"
	};

	TRex* trex_table[30];
	int i;
	int match = 0;

	for(i = 0; i < 28; i++) // trouver le moyen de staticifier ça
	{
		trex_table[i] = trex_compile(regex_strtable[i], NULL);
		if(trex_match(trex_table[i], mess_src))
		{
			match = 1;
			trex_free(trex_table[i]);
			break;
		}
		trex_free(trex_table[i]);
	}
	if(match == 0) i = -1;

	char * tmp = malloc((strlen(mess_src) + 1)* sizeof(char));
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
			mess_dest->node1 = strcopy(strtok(NULL, " "));
			mess_dest->node2 = strcopy(strtok(NULL, " "));
			mess_dest->n_parameter = atoi(strtok(NULL, " "));
			break;
		case 4:
			mess_dest->type = UPDATELINK;
			strtok(mess_src, " ");
			strtok(NULL, " ");
			mess_dest->node1  = strcopy(strtok(NULL, " "));
			mess_dest->node2  = strcopy(strtok(NULL, " "));
			mess_dest->n_parameter = atoi(strtok(NULL, " "));
			break;
		case 5:
			mess_dest->type = DELLINK;
			strtok(mess_src, " ");
			strtok(NULL, " ");
			mess_dest->node1  = strcopy(strtok(NULL, " "));
			mess_dest->node2  = strcopy(strtok(NULL, " "));
			break;
		case 6:
			mess_dest->type = DISCONNECT;
			break;
		case 7:
			mess_dest->type = MESSAGE;
			strcpy(tmp, mess_src);
			strtok(tmp, " ");

			mess_dest->node1  = strcopy(strtok(NULL, " "));

			char * ptr = strstr(mess_src, mess_dest->node1) + strlen(mess_dest->node1) + 2;
			mess_dest->s_parameter = malloc((1 + strlen(ptr)) * sizeof(char));
			strcpy(mess_dest->s_parameter, ptr);
			mess_dest->s_parameter[strlen(ptr) - 1] = 0;

			break;
		case 8:
			mess_dest->type = ROUTE;
			strtok(mess_src, " ");
			mess_dest->node1  = strcopy(strtok(NULL, " "));

			break;
		case 9:
			mess_dest->type = ROUTETABLE;
			break;
		case 10:
			mess_dest->type = LOGIN;
			strtok(mess_src, " ");
			strtok(NULL, " ");
			strtok(NULL, " ");
			mess_dest->node1  = strcopy(strtok(NULL, " "));
			strtok(NULL, " ");
			mess_dest->n_parameter = atoi(strtok(NULL, " "));

			break;
		case 11:
			mess_dest->type = LOGIN;
			//log in port p
			strtok(mess_src, " ");
			strtok(NULL, " ");
			strtok(NULL, " ");
			mess_dest->n_parameter = atoi(strtok(NULL, " "));

			break;
		case 12:
			mess_dest->type = LOGOUT;
			break;
		case 13:
			mess_dest->type = GREETING;
			strtok(mess_src, " ");
			mess_dest->node1  = strcopy(strtok(NULL, " "));
			break;
		case 14:
			mess_dest->type = BYE;
			break;
		case 15:
			mess_dest->type = POLL;
			break;
		case 16:
			mess_dest->type = NEIGHBORHOOD;
			strtok(mess_src, " ");
			strtok(NULL, " ");
			mess_dest->s_parameter  = strcopy(strtok(NULL, " "));
			break;
		case 17:
			mess_dest->type = NEIGHBORHOOD;
			mess_dest->accept = OK;
			break;
		case 18:
			mess_dest->type = LINK;
			break;
		case 19:
			mess_dest->type = LINK;
			mess_dest->accept = OK;
			break;
		case 20:
			mess_dest->type = VECTOR;
			strtok(mess_src, " ");
			mess_dest->s_parameter  = strcopy(strtok(NULL, " "));
			break;
		case 21:
			mess_dest->type = VECTOR;
			mess_dest->accept = OK;
			break;
		case 22:
			mess_dest->type = PACKET;

			strcpy(tmp, mess_src);
			strtok(tmp, " ");

			strtok(NULL, " ");
			mess_dest->node1  = strcopy(strtok(NULL, " "));
			strtok(NULL, " ");
			mess_dest->node2  = strcopy(strtok(NULL, " "));
			strtok(NULL, " ");

			char * tmp2 = strtok(NULL, " ");
			mess_dest->n_parameter = atoi(tmp2);

			mess_dest->s_parameter = strstr(mess_src, tmp2) + strlen(tmp2) + 1;

			break;
		case 23:
			mess_dest->type = PACKET;
			mess_dest->accept = OK;

			strtok(mess_src, " ");
			strtok(NULL, " ");
			mess_dest->node1  = strcopy(strtok(NULL, " "));
			strtok(NULL, " ");
			mess_dest->node2  = strcopy(strtok(NULL, " "));

			break;
		case 24:
			mess_dest->type = PACKET;
			mess_dest->accept = TOOFAR;

			strtok(mess_src, " ");
			strtok(NULL, " ");
			mess_dest->node1  = strcopy(strtok(NULL, " "));
			strtok(NULL, " ");
			mess_dest->node2  = strcopy(strtok(NULL, " "));
			break;
		case 25:
			mess_dest->type = PING;

			strtok(mess_src, " ");
			strtok(NULL, " ");
			mess_dest->node1  = strcopy(strtok(NULL, " "));
			strtok(NULL, " ");
			mess_dest->node2  = strcopy(strtok(NULL, " "));
			strtok(NULL, " ");
			mess_dest->n_parameter = atoi(strtok(NULL, " "));

			break;
		case 26:
			mess_dest->type = PONG;
			strtok(mess_src, " ");
			strtok(NULL, " ");
			mess_dest->node1  = strcopy(strtok(NULL, " "));
			strtok(NULL, " ");
			mess_dest->node2  = strcopy(strtok(NULL, " "));
			strtok(NULL, " ");
			mess_dest->n_parameter = atoi(strtok(NULL, " "));
			break;
		case 27:
			mess_dest->type = PING;
			strtok(mess_src, " ");
			mess_dest->node1  = strcopy(strtok(NULL, " "));
			break;
		default:
			mess_dest->type = NONE;
			fprintf(stderr, "ERREUR: Message invalide.\n");

			break;
	}

	free(tmp);
	return mess_dest;
}



void mess__free(struct Message** mess)
{
	if((*mess)->s_parameter != NULL)
	{
		free((*mess)->s_parameter);
	}
	if((*mess)->node1 != NULL)
	{
		free((*mess)->node1);
	}
	if((*mess)->node2 != NULL)
	{
		free((*mess)->node2);
	}

	free((*mess));
}
/*
int main(int argc, char * argv[])
{

	//mess__parse(m, argv[1], 4);
	char * truc = malloc(sizeof(char)*100);
	strcpy(truc, "message n12 \"salut les copains!\"");
	struct Message * m = mess__parse( truc);

	char* origin = "j'aime au//ssi l*a chatte \\ 7 * 3*";
	char* bidule = mess__escape(origin);
	char* chose = mess__unescape(bidule);
	printf("\n\noriginal: %s\néchappé: %s\noriginal: %s\n", origin, bidule, chose);

	free(truc);
	free(bidule);
	free(chose);
	mess__free(&m);
	return 0;
}
*/


void mess__debug(struct Message* m)
{
	if(m != NULL)
	{
		printf("Message: %d %d %d\n", m->type, m->n_parameter, m->accept);

		if(m->s_parameter != NULL)
		{
			printf("s_param: %s\n", m->s_parameter);
		}
		if(m->node1 != NULL)
		{
			printf("node1: %s\n", m->node1);
		}
		if(m->node2 != NULL)
		{
			printf("node2: %s\n", m->node2);
		}

		printf("\n\n");
	}
	else
	{
		printf("MESSAGE NUL RECU\n");
	}
}

//*///*/*/*////*/
/* Utilitaires */
//*///*/*/*////*/

char *str_sub (const char *s, unsigned int start, unsigned int end)
{
	char *new_s = NULL;

	if (s != NULL && start < end)
	{
		new_s = malloc (sizeof (*new_s) * (end - start + 2));
		if (new_s != NULL)
		{
			unsigned int i;

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

char* strcopy(const char* src)
{
	int n = strlen(src) +1;
	char* str = malloc(n  * sizeof(char));

	strcpy(str, src);
	return str;
}
