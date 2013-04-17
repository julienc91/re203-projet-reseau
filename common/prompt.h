#include <pthread.h>

#include "messages.h"
void* prompt__main_thread(void* v);
pthread_t*  prompt__start(void (*mess_handler) (struct Message*));
