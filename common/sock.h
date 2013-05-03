#ifndef SOCK_H_
#define SOCK_H_

#if defined(WIN32) || defined(__MINGW32__)

#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(__linux__)

#define h_addr h_addr_list[0] /*backward compatibility*/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
//~ #include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;


#else

#error not defined for this platform

#endif

#define CRLF		"\r\n"
#define BUF_SIZE	1024

#endif
