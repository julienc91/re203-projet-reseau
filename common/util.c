#include <string.h>
#include <stdlib.h>

void strcopy(char *dest, char* origin)
{
	free(dest);
	dest = malloc((strlen(origin) +1) * sizeof(char));
	strcpy(dest, origin);
}
