#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../trex/trex.h"

#include "config.h"
Configuration* config__readController(void)
{
	FILE * pFile;
	pFile = fopen ("controller.cfg","r");
	if (pFile != NULL)
	{
		Configuration* conf = malloc(sizeof(Configuration));
		conf->type = CONTROLLER;

		char * line = NULL;
		size_t len = 0;
		if(conf == NULL)
		{
			fprintf(stderr, "ERREUR : malloc\n");
			return NULL;
		}

		while (getline(&line, &len, pFile) != -1)
		{
			config__readLine(conf, line);
		}
		if (line)
			free(line);
	}
	else
	{
		fprintf(stderr, "ERREUR : Ouverture de controller.cfg impossible\n");
	}
}

Configuration* config__readRouter(void)
{
	FILE * pFile;
	pFile = fopen ("router.cfg","r");
	if (pFile != NULL)
	{
		Configuration* conf = malloc(sizeof(Configuration));
		conf->type = ROUTER;

		char * line = NULL;
		size_t len = 0;
		if(conf == NULL)
		{
			fprintf(stderr, "ERREUR : malloc\n");
			return NULL;
		}

		while (getline(&line, &len, pFile) != -1)
		{
			config__readLine(conf, line);
		}
		if (line)
			free(line);
	}
	else
	{
		fprintf(stderr, "ERREUR : Ouverture de router.cfg impossible\n");
	}
}

void config__readLine(Configuration* conf, char* mess_src)
{
	static char* regex_strtable[] =
	{
		"router-port = \\d*\\c",
		"controller-adress = \\d*.\\d*.\\d*.\\d*\\c",
		"controller-port = \\d*\\c",
		"controller-update-interval = \\d*\\c",
		"router-update-interval = \\d*\\c",
		"default-ttl-value = \\d*\\c",
		"default-ping-packet-count = \\d*\\c",
		"default-packet-timeout-value = \\d*\\c",
		"default-dv-timeout-value = \\d*\\c",
		"poll-timeout-value = \\d*\\c",
		"#.*\\c",
		"\\c",
		"truc quelconque et hautement improbable"
	};

	TRex* trex_current_regex;
	int i;
	int match = 0;

	for(i = 0; i < 12; i++) // trouver le moyen de staticifier ça
	{
		trex_current_regex = trex_compile(regex_strtable[i], NULL);
		if(trex_match(trex_current_regex, mess_src))
		{
			match = 1;
			trex_free(trex_current_regex);
			break;
		}
		trex_free(trex_current_regex);
	}
	mess_src[strlen(mess_src) - 1] = 0;

	if(match == 0) i = -1;

	char * tmp = malloc((strlen(mess_src) + 1)* sizeof(char));

	switch(i)
	{
		case 0:
			strtok(mess_src, " ");
			strtok(NULL, " ");

			conf->routerPort = atoi(strtok(NULL, " "));
			break;
		case 1:
			strtok(mess_src, " ");
			strtok(NULL, " ");

			strcpy(conf->controllerAddress, strtok(NULL, " "));
			break;
		case 2:
			strtok(mess_src, " ");
			strtok(NULL, " ");

			conf->controllerPort = atoi(strtok(NULL, " "));
			break;
		case 3:
			strtok(mess_src, " ");
			strtok(NULL, " ");

			conf->controllerUpdateInterval = atoi(strtok(NULL, " "));
			break;
		case 4:
			strtok(mess_src, " ");
			strtok(NULL, " ");

			conf->routerUpdateInterval = atoi(strtok(NULL, " "));
			break;
		case 5:
			strtok(mess_src, " ");
			strtok(NULL, " ");

			conf->defaultTTLValue = atoi(strtok(NULL, " "));
			break;
		case 6:
			strtok(mess_src, " ");
			strtok(NULL, " ");

			conf->defaultPingPacketCount = atoi(strtok(NULL, " "));
			break;
		case 7:
			strtok(mess_src, " ");
			strtok(NULL, " ");

			conf->defaultPacketTimeoutValue = atoi(strtok(NULL, " "));
			break;
		case 8:
			strtok(mess_src, " ");
			strtok(NULL, " ");

			conf->defaultDVTimeoutValue = atoi(strtok(NULL, " "));
			break;
		case 9:
			strtok(mess_src, " ");
			strtok(NULL, " ");

			conf->pollTimeoutValue = atoi(strtok(NULL, " "));
			break;
		case 10:
			// commentaires
			break;
		case 11:
			//lignes vides
		default:
			if(strcmp(mess_src, "") != 0)
			{
				fprintf(stderr, "ERREUR : ligne du fichier de configuration invalide : \"%s\".\n", mess_src);
			}
			break;
	}
}
