#ifndef _UTIL_H_
#define _UTIL_H_

#define DEBUG fprintf(stderr, "DEBUG : function : %s, at line %d, in file %s\n",__FUNCTION__, __LINE__ , __FILE__);

char *strcopy(char *origin);
void strcopy2(char **dest, char* origin);

#endif /*_UTIL_H_*/
