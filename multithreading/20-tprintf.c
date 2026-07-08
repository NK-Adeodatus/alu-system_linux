#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include "multithreading.h"

static pthread_mutex_t printf_mutex;

/**
 * init_mutex - Initializes the mutex before main executes
 */
void __attribute__((constructor)) init_mutex(void)
{
	pthread_mutex_init(&printf_mutex, NULL);
}

/**
 * destroy_mutex - Destroys the mutex after main finishes
 */
void __attribute__((destructor)) destroy_mutex(void)
{
	pthread_mutex_destroy(&printf_mutex);
}

/**
 * tprintf - prints a formatted string preceded by the calling thread ID
 * @format: format string, as used by the printf family
 *
 * Return: the number of characters printed, or a negative value
 * if an output error occurred
 */
int tprintf(char const *format, ...)
{
	va_list args;
	int ret;

	pthread_mutex_lock(&printf_mutex);

	ret = printf("[%lu] ", pthread_self());

	va_start(args, format);
	ret += vprintf(format, args);
	va_end(args);

	pthread_mutex_unlock(&printf_mutex);

	return (ret);
}
