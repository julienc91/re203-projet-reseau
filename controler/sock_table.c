#include "sock_table.h"
#include <stdio.h>

static hash_table_t *T;


void table__init(){

    T = hash_table_new(MODE_ALLREF);
}

void table__add_socket(char ** key, Client * c){

    if(!HT_HAS_KEY(T, *key))
        HT_ADD(T, key, c);
    else
        fprintf(stderr, "Key %s already exists\n", *key);
}

void table__delete_socket(char ** key){

    HT_REMOVE(T, *key);
}

void table__modify_key(char ** old_key, char ** new_key){
    Client * c = (Client *)HT_LOOKUP(T, *old_key);
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


Client * table__get_socket(char ** key){
    return (Client *)HT_LOOKUP(T, *key);
}

int table__has_socket(char ** key){
    return HT_HAS_KEY(T, *key);
}

void table__delete(){

    hash_table_delete(T);
}
