//functions used in net.h, for the controler
#ifndef _NET_FUNCTIONS_H_
#define _NET_FUNCTIONS_H_
#include "../common/net.h"

void input_event(network *, char *);
void connection_event(network *, Client *, char *);
void disconnection_event(network *, Client *);
void message_event(network *, Client *, char *);
#endif
