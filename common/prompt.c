/**
 * \file prompt.c
 * \brief A prompt system
 * 
 * Implements a prompt within a thread.
 * 
 */

#include "prompt.h"
#include "messages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *  \brief Start a prompt function in its own thread
 *  \param mess_handler Function used for managing the prompt's input.
 *  \return A pointer to the prompt's pthread.
 */
#ifdef __cplusplus
pthread_t* prompt__start(int (*mess_handler) (void*, struct Message*))
#else
     pthread_t* prompt__start(int (*mess_handler) (struct Message*))
#endif
{
     pthread_t* prompt_thread = malloc(sizeof(pthread_t));
     pthread_create(prompt_thread, NULL, prompt__main_thread, (void*)mess_handler);

     return prompt_thread;
}

/**
 *  \brief Prompt function
 *  \param v Function used for managing the prompt's input.
 *  \return NULL
 */
void* prompt__main_thread(void* v)
{
     char *s = malloc(256);
     struct Message* m = NULL;
     while((s = fgets(s, 255, stdin)) != NULL)
     {
	  s[strlen(s) - 1] = 0;
	  m = mess__parse(s);

	  //mess__debug(m);
	  if(v != NULL && m != NULL)
	  {
	       int r;
#ifdef __cplusplus
	       r = ((int (*)(void*, struct Message*)) v)(m);
#else
	       r = ((int (*)(struct Message*)) v)(m);
#endif
	       if (r == 1)
	       {
		    mess__free(&m);
		    free(s);
		    pthread_exit(NULL); 
	       }
	  }
        
	  mess__free(&m);
     }

     free(s);
     return NULL;
}
