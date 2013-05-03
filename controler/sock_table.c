#include "sock_table.h"
#include <stdio.h>
#include <string.h>

static hash_table_t *T;


void table__init(){

    T = hash_table_new(MODE_ALLREF);
    //~ T = hash_table_new(MODE_VALUEREF);
}

void table__add_info(char **key, Client_info **c){


    if(!hash_table_has_key(T, *key, strlen(*key)))
        hash_table_add(T,*key, strlen(*key), *c, sizeof(*c));
    else
        fprintf(stderr, "Key %s already exists\n", *key);
}

void table__delete_info(char **key){

    hash_table_remove(T, *key, strlen(*key));
}

void table__modify_key(char **old_key, char **new_key){
    Client_info *c = (Client_info *)hash_table_lookup(T, *old_key, strlen(*old_key));
    if (c != NULL){
        if(!hash_table_has_key(T, *new_key, strlen(*new_key))){
            hash_table_remove(T, *old_key, strlen(*old_key));
            hash_table_add(T, *new_key, strlen(*new_key), (void *) c, sizeof(c));
        }
        else
            fprintf(stderr, "Key %s already exists\n", *new_key);
    }
    else
        fprintf(stderr, "Key %s does not exist\n", *old_key);
}


Client_info *table__get_info(char **key){
    return (Client_info *)hash_table_lookup(T, *key, strlen(*key));
}

int table__has_info(char **key){
    return hash_table_has_key(T, *key, strlen(*key));
}

void table__delete(){

    hash_table_delete(T);
}
