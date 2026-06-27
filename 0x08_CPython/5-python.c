#include <Python.h>
#include <longintrepr.h>
#include <limits.h>

void print_python_int(PyObject *p);

/**
 * print_python_int - Prints basic info about Python int objects
 * @p: PyObject int
 */
void print_python_int(PyObject *p)
{
	PyLongObject *num = (PyLongObject *)p;
	Py_ssize_t size, abs_size, i;
	unsigned long result = 0;
	digit d;
	int is_negative, overflow = 0;

	if (!PyLong_Check(p))
	{
		printf("Invalid Int Object\n");
		fflush(stdout);
		return;
	}

	size = num->ob_base.ob_size;
	is_negative = size < 0;
	abs_size = is_negative ? -size : size;

	for (i = abs_size - 1; i >= 0; i--)
	{
		d = num->ob_digit[i];
		if (result > (ULONG_MAX - d) / ((unsigned long)1 << PyLong_SHIFT))
		{
			overflow = 1;
			break;
		}
		result = (result << PyLong_SHIFT) + d;
	}

	if (overflow)
	{
		printf("C unsigned long int overflow\n");
		fflush(stdout);
		return;
	}

	if (is_negative)
		printf("-%lu\n", result);
	else
		printf("%lu\n", result);
	fflush(stdout);
}
