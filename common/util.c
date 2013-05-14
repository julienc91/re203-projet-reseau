/**
 * \file util.c
 * \brief Useful functions
 * 
 * A set of useful functions used in the common code.
 * 
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"

/**
 *  \brief Copy a string to another
 *  \param dest Destination string.
 *  \param origin The string to copy.
 */
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

/**
 *  \brief Copy a string to another
 *  \param origin The string to copy.
 *  \return A copy of the origin string.
 */
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
