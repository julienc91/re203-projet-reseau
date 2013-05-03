#ifndef _EXEC_H
#define _EXEC_H
#include "../common/net.h"
#include "../common/messages.h"
#include "../common/config.h"


void exec__init(void);
void exec__prompt_message(struct Message* m);
struct Message *exec__sock_message(struct Message* m);
network *net;
static Configuration *config;
#endif

