#include "prompt.h"

int main(void)
{
	pthread_t* prompt_th = prompt__start();
	pthread_join(*prompt_th, NULL);

}
