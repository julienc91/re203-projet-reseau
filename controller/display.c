/**
 * \file display.c
 * \brief Display messages
 * 
 * Sends messages to the user via the prompt.
 * 
 */

#include <stdio.h>

/**
 *  \brief Message shown after the "load" command
 *  \param n Number of nodes in the network.
 */
void disp__loaded(int n)
{
	printf("    -> Topology loaded (%d nodes in the network) !\n", n);
}

/**
 *  \brief Message shown after the "save" command
 *  \param n Number of nodes in the network.
 */
void disp__saved(int n)
{
	printf("    -> Topology saved !(%d nodes in the network)\n", n);
}

/**
 *  \brief Message shown after the "add link" command
 *  \param n1 First node of the new link.
 *  \param n2 Second node of the new link.
 *  \param w Weight of the new link.
 */
void disp__addlink(char* n1, char* n2, int w)
{
	printf("    -> Link created between %s and %s. Link cost = %d.\n", n1, n2, w);
}

/**
 *  \brief Message shown after the "update link" command
 *  \param n1 First node of the updated link.
 *  \param n2 Second node of the updated link.
 *  \param w Weight of the updated link.
 */
void disp__updlink(char* n1, char* n2, int w)
{
	printf("    -> Link updated between %s and %s. Link cost = %d.\n", n1, n2, w);
}

/**
 *  \brief Message shown after the "delete link" command
 *  \param n1 First node of the deleted link.
 *  \param n2 Second node of the deleted link.
 */
void disp__dellink(char* n1, char* n2)
{
	printf("    -> Link deleted between %s and %s.\n", n1, n2);
}

/**
 *  \brief Message shown after the "disconnect" command
 *  \param n The disconnected router's id.
 */
void disp__disconnect(char* n)
{
	printf("    -> Node %s is disconnected !\n", n);
}
