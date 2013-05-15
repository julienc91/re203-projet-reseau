/**
 * \file messages.c
 * \brief Data structure for messages between controller/router/user
 *
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../trex/trex.h"
#include "messages.h"
#include "util.h"

#define MESS__NB_REGEXES 33

int isEverythingInitialized = 0;
TRex** trex_regexes;
static char* regex_strtable[] =
{
	"load .*",
	"save .*",
	"show topology",
	"add link \\w* \\w* \\d*",
	"update link \\w* \\w* \\d*",
	"del link \\w* \\w*",
	"disconnect \\w*",
	"message \\w* \".*\"",
	"route \\w*",
	"routetable",
	"log in as \\w* port \\d*",
	"log in port \\d*",
	"log out",
	"greeting \\w*",
	"bye",
	"poll",
	"neighborhood newlist \\[.*\\]", //pourrait être amélioré pour vérifier directement si c'est bon
	"neighborhood ok",
	"link id \\w*",
	"link ok",
	"vector \\[.*\\]",
	"vector ok",
	"packet seqnum \\d* src \\w* dst \\w* ttl \\d* data .*",
	"packet seqnum \\d* src \\w* dst \\w* ok",
	"packet seqnum \\d* src \\w* dst \\w* toofar",
	"ping seqnum \\d* src \\w* dst \\w* ttl \\d*",
	"pong seqnum \\d* src \\w* dst \\w* ttl \\d*",
	"pong seqnum \\d* src \\w* dst \\w* ttlzero",
	"ping \\w*",
	"quit",
	"neighborhood newlist \\[\\]",
	"vector \\[\\]",
	"debug"
};

char *str_sub (const char *s, unsigned int start, unsigned int end);

/**
 *  \brief Initialize the message system
 */
void mess__base__init(void)
{
	int i;
	trex_regexes = malloc(sizeof(TRex*) * MESS__NB_REGEXES);
	for(i = 0; i < MESS__NB_REGEXES; i++) // trouver le moyen de staticifier ça
	{
		trex_regexes[i] = trex_compile(regex_strtable[i], NULL);
	}
	isEverythingInitialized = 1;
}

/**
 * \brief Free memory.
 */
void mess__base__free(void)
{
    int i;
    for (i = 0; i < MESS__NB_REGEXES; i++)
        trex_free(trex_regexes[i]);
    free (trex_regexes);
}

/**
 *  \brief Creates a new empty message
 *  \param mess Variable to initialize.
 */
void mess__init(struct Message** mess)
{
	if(!isEverythingInitialized) mess__base__init();
     *mess = malloc(sizeof(struct Message));
     (*mess)->type = NONE;
     (*mess)->s_parameter = NULL;
     (*mess)->node1 = NULL;
     (*mess)->node2 = NULL;
     (*mess)->n_parameter = -1;
     (*mess)->seqnum = -1;
     (*mess)->accept = NOT;
}

/**
 *  \brief Get a message's weight
 *  \param mess Message
 *  \return WEIGHT_ERROR if an error occured, the message's weight otherwise.
 */
int mess__getWeight(struct Message* mess)
{
	if(!isEverythingInitialized) mess__base__init();
     if(!mess)
	  return WEIGHT_ERROR;
     return mess->n_parameter;
}

/**
 *  \brief Get a message's TTL
 *  \param mess Message
 *  \return TTL_ERROR if an error occured, the message's TTL otherwise.
 */
int mess__getAndDecTTL(struct Message* mess)
{
	if(!isEverythingInitialized) mess__base__init();
     if(!mess)
	  return TTL_ERROR;
     return (mess->n_parameter)--;
}

/**
 *  \brief Get a message's acceptance
 *  \param mess Message
 *  \return ACCEPTANCE_ERROR if an error occured, the message's acceptance
 *   otherwise.
 */
int mess__getAcceptance(struct Message* mess)
{
	if(!isEverythingInitialized) mess__base__init();
     if(!mess)
	  return ACCEPTANCE_ERROR;
     return mess->accept;
}

/**
 * \brief Escape a string from special chars
 * \param mess_src The string to escape.
 * \return A new string containing the original string once escaped.
 *
 * Should be used before sending any message.
 */
char* mess__escape(char* mess_src)
{
	if(!isEverythingInitialized) mess__base__init();
     if(!mess_src)
	  return NULL;
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

/**
 * \brief Remove escape chars from a string
 * \param mess_src The string to unescape.
 * \return A new string containing the original string once unescaped.
 *
 * Should be used with messages received by the host.
 */
char* mess__unescape(char* mess_src)
{
	if(!isEverythingInitialized) mess__base__init();
     if(!mess_src)
	  return NULL;
     int count = 0, n = strlen(mess_src);
     char *mess_unescp = malloc((n+1) * sizeof(char)); // TODO: à affiner

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

/**
 * \brief Parse a multiline string
 * \param mess_src The string to parse.
 * \return A Message created from the parameter.
 */
Messages *mess__multiline_parse(char *mess_src)
{
	if(!isEverythingInitialized) mess__base__init();
     if(!mess_src)
	  return NULL;
     Messages *m = malloc(sizeof(*m));
     m->messages = malloc(sizeof(*(m->messages)) * MESSAGE__MAX_MESSAGES);
     m->nb_messages = 0;

     int count = 0, n = strlen(mess_src);
     char* mess_unescp = malloc((n+1) * sizeof(char)); // TODO: à affiner

     for(int i = 0; i < n; i++)
     {
	  mess_unescp[count++] = mess_src[i];

	  if(mess_src[i] == '\n' ||
	     (i > 0 && (mess_src[i-1] =! '\\' && i+1 <= n && mess_src[i] == '*')))
	  {

	       mess_unescp[count] = '\0';
	       count = 0;

	       if (m->nb_messages + 1 >= MESSAGE__MAX_MESSAGES)
	       {
		    fprintf(stderr,"Error: messages limit reached.\n");

		    break;
	       }
           char *tmp = mess__treatInput(mess_unescp);
	       m->messages[m->nb_messages++] = mess__parse(tmp);
           free(tmp);
	  }

     }

     if (count > 0)
     {
	  mess_unescp[count] = '\0';
	  if (m->nb_messages + 1 >= MESSAGE__MAX_MESSAGES)
	  {
	       fprintf(stderr,"Error: messages limit reached.\n");
	  }
	  else
	  {
          char *tmp = mess__treatInput(mess_unescp);
	       m->messages[m->nb_messages++] = mess__parse(tmp);
           free(tmp);
	  }
     }

     free(mess_unescp);

     return m;
}

/**
 * \brief Parse a string
 * \param mess_src The string to parse.
 * \return A Message created from the parameter.
 */
struct Message* mess__parse(char* mess_src)
{
	if(!isEverythingInitialized) mess__base__init();
     if(!mess_src)
	  return NULL;
     struct Message* mess_dest;
     mess__init(&mess_dest);

     int i;
     int match = 0;
     char * ptr, *tmp2;

     for(i = 0; i < MESS__NB_REGEXES; i++) // trouver le moyen de staticifier ça
     {
	  if(trex_match(trex_regexes[i], mess_src))
	  {
	       match = 1;
	       break;
	  }
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
	  strtok(mess_src, " ");
	  mess_dest->node1  = strcopy(strtok(NULL, " "));
	  break;
     case 7:
	  mess_dest->type = MESSAGE;
	  strcpy(tmp, mess_src);
	  strtok(tmp, " ");

	  mess_dest->node1  = strcopy(strtok(NULL, " "));

	  ptr = strstr(mess_src, mess_dest->node1) + strlen(mess_dest->node1) + 2;
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
	  strtok(mess_src, " ");
	  strtok(NULL, " ");
	  mess_dest->node1  = strcopy(strtok(NULL, " "));
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
	  mess_dest->seqnum  = atoi(strtok(NULL, " "));
	  strtok(NULL, " ");
	  mess_dest->node1  = strcopy(strtok(NULL, " "));
	  strtok(NULL, " ");
	  mess_dest->node2  = strcopy(strtok(NULL, " "));
	  strtok(NULL, " ");

	  tmp2 = strtok(NULL, " ");
	  mess_dest->n_parameter = atoi(tmp2);


	  mess_dest->s_parameter = strcopy(strtok(NULL, "\0") + 5); //strstr(mess_src, tmp2) + strlen(tmp2) + 1;
	  break;

     case 23:
	  mess_dest->type = PACKET;
	  mess_dest->accept = OK;

	  strtok(mess_src, " ");
	  strtok(NULL, " ");
	  mess_dest->seqnum  = atoi(strtok(NULL, " "));
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
	  mess_dest->seqnum  = atoi(strtok(NULL, " "));
	  strtok(NULL, " ");
	  mess_dest->node1  = strcopy(strtok(NULL, " "));
	  strtok(NULL, " ");
	  mess_dest->node2  = strcopy(strtok(NULL, " "));
	  break;

     case 25:
	  mess_dest->type = PING;

	  strtok(mess_src, " ");
	  strtok(NULL, " ");
	  mess_dest->seqnum  = atoi(strtok(NULL, " "));
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
	  mess_dest->seqnum  = atoi(strtok(NULL, " "));
	  strtok(NULL, " ");
	  mess_dest->node1  = strcopy(strtok(NULL, " "));
	  strtok(NULL, " ");
	  mess_dest->node2  = strcopy(strtok(NULL, " "));
	  strtok(NULL, " ");
	  mess_dest->n_parameter = atoi(strtok(NULL, " "));
	  break;

     case 27:
	  mess_dest->type = PONG;
	  mess_dest->accept = TTLZERO;

	  strtok(mess_src, " ");
	  strtok(NULL, " ");
	  mess_dest->seqnum  = atoi(strtok(NULL, " "));
	  strtok(NULL, " ");
	  mess_dest->node1  = strcopy(strtok(NULL, " "));
	  strtok(NULL, " ");
	  mess_dest->node2  = strcopy(strtok(NULL, " "));
	  break;

     case 28:
	  mess_dest->type = PING;
	  strtok(mess_src, " ");
	  mess_dest->node1  = strcopy(strtok(NULL, " "));
	  break;

     case 29:
	  mess_dest->type = QUIT;
	  break;

     case 30:
	  mess_dest->type = NEIGHBORHOOD;
	  mess_dest->s_parameter = malloc(sizeof(char));
	  mess_dest->s_parameter[0] = 0;
	  break;

     case 31:
	  mess_dest->type = VECTOR;
	  mess_dest->s_parameter = malloc(sizeof(char) * 3);
	  mess_dest->s_parameter[0] = '[';
	  mess_dest->s_parameter[1] = ']';
	  mess_dest->s_parameter[2] = 0;
	  break;

     case 32:
       	  mess_dest->type = NETWORK_DEBUG;
       break;

     default:
	  mess_dest->type = NONE;
	  fprintf(stderr, "ERREUR: Message invalide : '%s'.\n", mess_src);

	  break;
     }

     free(tmp);
     return mess_dest;
}

/**
 * \brief Convert a message into a string
 * \param mess The message to convert.
 * \return A string corresponding to the message's type.
 */
char* mess__toString(struct Message* mess)
{
	if(!isEverythingInitialized) mess__base__init();
     if(!mess)
	  return NULL;
     char * out = malloc(sizeof(char) * (50 + ((mess->s_parameter != NULL)? strlen(mess->s_parameter) : 0)));

     switch(mess->type)
     {
	  // Routeur <-> Controleur
     case LOGIN: 			//log in as ID port p
	  if(mess->node1 == NULL)
	  {
	       sprintf(out, "log in port %d", mess->n_parameter);
	  }
	  else
	  {
	       sprintf(out, "log in as %s port %d", mess->node1, mess->n_parameter);
	  }
	  break;
	  //log in port p
     case LOGOUT: 		//log out
	  sprintf(out, "log out");
	  break;

     case GREETING: 		//greeting n  (que se passe-t-il si rien libre?)
	  sprintf(out, "greeting %s", mess->node1);
	  break;

     case BYE:		//bye
	  sprintf(out, "bye");
	  break;

     case POLL:			//poll
	  sprintf(out, "poll");
	  break;

     case NEIGHBORHOOD:
	  if(mess->accept == OK)
	       sprintf(out, "neighborhood ok");
	  else
	       sprintf(out, "neighborhood newlist %s", mess->s_parameter);
	  break;

     case LINK:
	  if(mess->accept == OK)
	       sprintf(out, "link ok");
	  else
	       sprintf(out, "link id %s", mess->node1);
	  break;

     case VECTOR:
	  if(mess->accept == OK)
	       sprintf(out, "vector ok");
	  else
	       sprintf(out, "vector %s", mess->s_parameter);
	  break;

     case PACKET:
	  if(mess->accept == OK)
	       sprintf(out, "packet seqnum %d src %s dst %s ok", mess->seqnum, mess->node1, mess->node2);
	  else if(mess->accept == TOOFAR)
	       sprintf(out, "packet seqnum %d src %s dst %s toofar", mess->seqnum, mess->node1, mess->node2);
	  else
	       sprintf(out, "packet seqnum %d src %s dst %s ttl %d data %s", mess->seqnum, mess->node1, mess->node2, mess->n_parameter, mess->s_parameter);
	  break;

     case PING: // penser aux deux ping, haut niveau et bas niveau
	  sprintf(out, "ping seqnum %d src %s dst %s ttl %d", mess->seqnum, mess->node1, mess->node2, mess->n_parameter);
	  break;

     case PONG:
	  if(mess->accept != TTLZERO)
	       sprintf(out, "pong seqnum %d src %s dst %s ttl %d", mess->seqnum, mess->node1, mess->node2, mess->n_parameter);
	  else
	       sprintf(out, "pong seqnum %d src %s dst %s ttlzero", mess->seqnum, mess->node1, mess->node2);
	  break;

     default:
	  fprintf(stderr, "ERREUR: Message invalide.\n");
	  break;

     }

     return out;
}

/**
 * \brief Free a Message from the memory
 * \param mess The message to free.
 */
void mess__free(struct Message** mess)
{
	if(!isEverythingInitialized) mess__base__init();
     if (*mess == NULL) return;

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
     *mess = NULL;
}

/**
 * \brief Free a Messages from the memory
 * \param m The messages to free.
 */
void mess__free_messages(Messages **m)
{
	if(!isEverythingInitialized) mess__base__init();
     if (*m == NULL) return;

     unsigned int i;
     for (i = 0; i < (*m)->nb_messages; i++)
     {
	  if ((*m)->messages[i] != NULL)
	       mess__free(&((*m)->messages[i]));
     }
     free((*m)->messages);
     free(*m);
     *m = NULL;
}

/**
 * \brief Debug function
 * \param m The messages to work with.
 *
 * Should not be used by the user.
 */
void mess__debug(struct Message* m)
{
	if(!isEverythingInitialized) mess__base__init();
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

/**
 * \brief Treat an input message
 * \param src The message received.
 * \return A new string containing the unescaped message and without
 *  the final '*'.
 */
char* mess__treatInput(char * src)
{
	if(!isEverythingInitialized) mess__base__init();
     if(!src)
	  return NULL;
     // On enlève les \\ et \*
     src = mess__unescape(src);

     // On enlève l'étoile de fin
     if(src[strlen(src) - 1] == '*')
	  src[strlen(src) - 1] = 0;

     return src;
}

/**
 * \brief Treat an output message
 * \param src The message to send.
 * \return A new string containing the escaped message and with
 *  the final '*'.
 */
char* mess__treatOutput(char * src)
{
	if(!isEverythingInitialized) mess__base__init();
     if(!src)
	  return NULL;
     // On rajoute les \\ et \*
     src = mess__escape(src);

     // On rajoute l'étoile de fin
     char *final = malloc((strlen(src) + 2) * sizeof(char));
     strcpy(final, src);

     final[strlen(src)] = '*';
     final[strlen(src) + 1] = '\0';

     free(src);
     return final;
}


//*///*/*/*////*/
/* Utilitaires */
//*///*/*/*////*/

/**
 * \brief Extract a substring from a string
 * \param s The string to work with.
 * \param start The index of the first char in the substring.
 * \param end The index of the last char in the substring.
 * \return A new string containing the wanted substring.
 */
char *str_sub (const char *s, unsigned int start, unsigned int end)
{
     if(!s)
	  return NULL;
     char *new_s = NULL;

     if (s != NULL && start < end && end <= strlen(s))
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
