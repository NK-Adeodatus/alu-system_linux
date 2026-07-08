#include <stdio.h>
#include <pthread.h>
#include "multithreading.h"

/**
 * thread_entry - entry point for a new thread, prints a string
 * @arg: address of the string to print, followed by a new line
 *
 * Return: NULL, this function never actually returns since it
 * terminates the calling thread via pthread_exit
 */
void *thread_entry(void *arg)
{
	printf("%s\n", (char *)arg);
	pthread_exit(NULL);

	return (NULL);
}
