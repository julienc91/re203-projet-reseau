#ifndef _EXEC_H
#define _EXEC_H
#include "../common/net.h"
#include "../common/messages.h"
#include "../common/config.h"

#define GRAPH__UNLOADED_ERROR -1

network *net;
static Configuration *config;

void exec__init(void);
int exec__prompt_message(struct Message* m);
struct Message *exec__sock_message(struct Message* m);

#endif

