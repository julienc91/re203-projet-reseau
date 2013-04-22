#ifndef _SOCK_TABLE_H

#include "../src/net.h"
#include "hashtable.h"

void init_table();

void add_socket(char * key, Client * c);

void delete_sockect(char * key);

void modify_key(char * old_key, char * new_key);

Client * get_socket(char * key);

int has_socket(char * key);

void delete_table();





#endif //_SOCK_TABLE_H
