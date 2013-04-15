#include <stdlib.h>
#include <stdio.h>
#include "../trex/trex.h"
#include "messages.h"
void mess__parse(struct Message* mess_dest, char* mess_src, size_t len)
{
	static char* regex_strtable[] =
	{
		"load [\\w\\.]*",
		"save [\\w\\.]*",
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
	for(int i = 0; i < 26; i++)
	{
		trex_table[i] = trex_compile(regex_strtable[i], NULL);
		if(trex_match(trex_table[i], mess_src))
			printf("\n\nLigne: %d\n", i);
	}
//	mess_dest = malloc(sizeof(struct Message));




}


int main(int argc, char * argv[])
{
	struct Message * m;
	mess__parse(m, argv[1], 4);
	return 0;
}
