#include "sock_table.h"
#include <stdio.h>
#include <string.h>

static hash_table_t *T;


void table__init(){

    T = hash_table_new(MODE_ALLREF);
    //~ T = hash_table_new(MODE_VALUEREF);
}

void table__add_info(char **key, Client_info *c){


    if(!hash_table_has_key(T, *key, strlen(*key)))
        HT_ADD(T, *key, c);
    else
        fprintf(stderr, "Key %s already exists\n", *key);
}

void table__delete_info(char **key){

    HT_REMOVE(T, *key);
}

void table__modify_key(char **old_key, char **new_key){
    Client_info *c = (Client_info *)HT_LOOKUP(T, *old_key);
    if (c != NULL){
        if(!HT_HAS_KEY(T, *new_key)){
            HT_REMOVE(T, *old_key);
            HT_ADD(T, *new_key, c);
        }
        else
            fprintf(stderr, "Key %s already exists\n", *new_key);
    }
    else
        fprintf(stderr, "Key %s does not exist\n", *old_key);
}


Client_info *table__get_info(char **key){
    return (Client_info *)HT_LOOKUP(T, *key);
}

int table__has_info(char **key){
    return HT_HAS_KEY(T, *key);
}

void table__delete(){

    hash_table_delete(T);
}
