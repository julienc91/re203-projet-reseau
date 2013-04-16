#include "prompt.h"
#include "../messages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void promp_start(void)
{
	char * s = malloc(256);
	struct Message* m;
	while((s = fgets(s, 255, stdin)) != NULL)
	{
		s[strlen(s) - 1] = 0;
		m = mess__parse(s);

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
}



int main(void)
{
	promp_start();
}
