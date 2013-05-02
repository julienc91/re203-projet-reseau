#ifndef _SOCK_TABLE_H
#define _SOCK_TABLE_H
#include "../common/net.h"
#include "hashtable.h"

typedef struct {
	char * address;
	int port;
} Client_info;

/*initialize the hashtable for the socket storage
 */
void table__init();

/*add the Client * c in the hashtable, with key index
 */
void table__add_socket(char ** key, Client * c);

/*remove the key indexed Client from the hashtable
 */
void table__add_info(char **key, Client_info **c);

/*remove the key indexed Client from the hashtable
 */
void table__delete_info(char ** key);

/*change to new_key, the key of the element pointed by old_key in the hashtable
 */
void table__modify_key(char ** old_key, char ** new_key);

/* return a pointer to the Client element in the hashtable
 */
Client_info *table__get_info(char ** key);

/* return 1 if key exist in the hashtable,
 * 0 if not
 */

int table__has_info(char ** key);

/*clear the hashtable
 */
void table__delete();


#endif //_SOCK_TABLE_H
