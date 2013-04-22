#ifndef _SOCK_TABLE_H
#define _SOCK_TABLE_H
#include "../src/net.h"
#include "hashtable.h"


/*initialize the hashtable for the socket storage
 */
void table__init();

/*add the Client * c in the hashtable, with key index
 */
 void table__add_socket(char * key, Client * c);

/*remove the key indexed Client from the hashtable
 */
void table__delete_socket(char * key);

/*change to new_key, the key of the element pointed by old_key in the hashtable
 */
void table__modify_key(char * old_key, char * new_key);

/* return a pointer to the Client element in the hashtable
 */
Client * table__get_socket(char * key);

/* return 1 if key exist in the hashtable,
 * 0 if not
 */
int table__has_socket(char * key);

/*clear the hashtable
 */
void table__delete();


#endif //_SOCK_TABLE_H
