/**
 * \file info_table.c
 * \brief The Client_info table
 * 
 * A hashtable linking a client to its Client_info structure
 * 
 */

#include "info_table.h"
#include <stdio.h>
#include <string.h>

static hash_table_t *T;

/**
 *  \brief Initialization
 *
 */
void table__init(){

    T = hash_table_new(MODE_ALLREF);
}

/**
 *  \brief Add a client into the table
 *  \param key The client's id.
 *  \param c The client's Client_info structure.
 */
void table__add_info(char **key, Client_info **c){


    if(!hash_table_has_key(T, *key, strlen(*key)))
        hash_table_add(T,*key, strlen(*key), *c, sizeof(*c));
    else
        fprintf(stderr, "Key %s already exists\n", *key);
}

/**
 *  \brief Delete a client from the table
 *  \param key The client's id.
 *
 */
void table__delete_info(char **key){

    hash_table_remove(T, *key, strlen(*key));
}

/**
 *  \brief Modify a client's id in the table.
 *  \param old_key The old id.
 *  \param new_key The new id.
 *
 */
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

/**
 *  \brief Get a client's Client_info structure
 *  \param key The client's id.
 *  \return A pointer to the wanted Client_info.
 *
 */
Client_info *table__get_info(char **key){
    return (Client_info *)hash_table_lookup(T, *key, strlen(*key));
}

/**
 *  \brief Tell if a client is already in the table or not
 *  \param key The client's id.
 *  \return 1 if the client is in the table, 0 otherwise.
 *
 */
int table__has_info(char **key){
    return hash_table_has_key(T, *key, strlen(*key));
}

/**
 *  \brief Delete the table
 *
 */
void table__delete(){
    hash_table_delete(T);
}
