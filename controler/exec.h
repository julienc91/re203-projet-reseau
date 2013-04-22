#ifndef _EXEC_H
#define _EXEC_H
#include "../src/net.h"
#include "../common/messages.h"
static network *net;

void exec__init();
void exec__prompt_message(struct Message* m);
void exec__sock_message(struct Message* m);

#endif
