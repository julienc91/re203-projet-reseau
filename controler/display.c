#include <stdio.h>

void disp__loaded(int n)
{
	printf("    -> Topology loaded (%d nodes in the network) !\n", n);
}

void disp__saved(int n)
{
	printf("    -> Topology saved !(%d nodes in the network)\n", n);
}

void disp__topology(void)
{
	printf("TODO\n");
}

void disp__addlink(char* n1, char* n2, int w)
{
	printf("    -> Link created between %s and %s. Link cost = %d.", n1, n2, w);
}

void disp__updlink(char* n1, char* n2, int w)
{
	printf("    -> Link updated between %s and %s. Link cost = %d.", n1, n2, w);
}

void disp__dellink(char* n1, char* n2)
{
	printf("    -> Link deleted between %s and %s.", n1, n2);
}

void disp__disconnect(char* n)
{
	printf("    -> Node %s is disconnected !", n);
}
