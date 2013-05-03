#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void strcopy2(char **dest, char* origin)
{
	if (*dest != NULL)
	{
		free(*dest);
	}
	if (origin != NULL)
	{
	*dest = malloc((strlen(origin) +1) * sizeof(char));
	strcpy(*dest, origin);
	}
	else
	{
		*dest = NULL;
	}
}

char *strcopy( const char* origin)
{
	char * dest;
	if (origin != NULL)
	{
	dest = malloc((strlen(origin) +1) * sizeof(char));
	strcpy(dest, origin);
	}
	else
	{
		dest = NULL;
	}
	return dest;
}
