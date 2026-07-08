#include "multithreading.h"
#include <stdlib.h>

/**
 * add_factor - dynamically allocates and adds a factor to the list
 * @list: pointer to the list
 * @n: factor to add
 *
 * Return: 1 on success, 0 on failure
 */
static int add_factor(list_t *list, unsigned long n)
{
	unsigned long *factor;

	factor = malloc(sizeof(*factor));
	if (!factor)
		return (0);
	*factor = n;
	list_add(list, factor);
	return (1);
}

/**
 * prime_factors - factorizes a number into a list of prime factors
 * @s: string representation of the number to factorize
 *
 * Return: list of prime factors, or NULL on failure
 */
list_t *prime_factors(char const *s)
{
	unsigned long n, i;
	list_t *list;

	if (!s)
		return (NULL);

	n = strtoul(s, NULL, 10);
	list = calloc(1, sizeof(*list));
	if (!list || n < 2)
		return (list);

	while (n % 2 == 0)
	{
		if (!add_factor(list, 2))
			return (NULL);
		n /= 2;
	}

	for (i = 3; i <= n / i; i += 2)
	{
		while (n % i == 0)
		{
			if (!add_factor(list, i))
				return (NULL);
			n /= i;
		}
	}

	if (n > 2 && !add_factor(list, n))
		return (NULL);

	return (list);
}
