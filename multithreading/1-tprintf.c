#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include "multithreading.h"

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

	ret = printf("[%lu] ", pthread_self());

	va_start(args, format);
	ret += vprintf(format, args);
	va_end(args);

	return (ret);
}
