#include "multithreading.h"
#include <stdlib.h>

/**
 * prime_factors - factorizes a number into a list of prime factors
 * @s: string representation of the number to factorize
 *
 * Return: list of prime factors, or NULL on failure
 */
list_t *prime_factors(char const *s)
{
	unsigned long n, i, *factor;
	list_t *list;

	if (!s)
		return (NULL);

	n = strtoul(s, NULL, 10);
	list = calloc(1, sizeof(*list));
	if (!list)
		return (NULL);
	if (n < 2)
		return (list);

	while (n % 2 == 0)
	{
		factor = malloc(sizeof(*factor));
		if (!factor)
			return (NULL);
		*factor = 2;
		list_add(list, factor);
		n /= 2;
	}

	for (i = 3; i <= n / i; i += 2)
	{
		while (n % i == 0)
		{
			factor = malloc(sizeof(*factor));
			if (!factor)
				return (NULL);
			*factor = i;
			list_add(list, factor);
			n /= i;
		}
	}

	if (n > 2)
	{
		factor = malloc(sizeof(*factor));
		if (!factor)
			return (NULL);
		*factor = n;
		list_add(list, factor);
	}

	return (list);
}
