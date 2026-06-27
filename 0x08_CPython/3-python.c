#include <Python.h>
#include <listobject.h>
#include <bytesobject.h>
#include <floatobject.h>
#include <string.h>
#include <stdlib.h>

void print_python_list(PyObject *p);
void print_python_bytes(PyObject *p);
void print_python_float(PyObject *p);
void format_python_float(double val, char *buf);

/**
 * print_python_list - Prints basic info about Python lists
 * @p: PyObject list
 */
void print_python_list(PyObject *p)
{
	Py_ssize_t size, alloc, i;
	PyListObject *list = (PyListObject *)p;
	PyObject *item;

	if (!PyList_Check(p))
	{
		printf("  [ERROR] Invalid List Object\n");
		fflush(stdout);
		return;
	}

	size = list->ob_base.ob_size;
	alloc = list->allocated;

	printf("[*] Python list info\n");
	printf("[*] Size of the Python List = %ld\n", size);
	printf("[*] Allocated = %ld\n", alloc);

	for (i = 0; i < size; i++)
	{
		item = list->ob_item[i];
		printf("Element %ld: %s\n", i, item->ob_type->tp_name);
		if (strcmp(item->ob_type->tp_name, "bytes") == 0)
			print_python_bytes(item);
		else if (strcmp(item->ob_type->tp_name, "float") == 0)
			print_python_float(item);
	}
	fflush(stdout);
}

/**
 * print_python_bytes - Prints basic info about Python bytes objects
 * @p: PyObject bytes
 */
void print_python_bytes(PyObject *p)
{
	Py_ssize_t size, i, limit;
	PyBytesObject *bytes = (PyBytesObject *)p;

	printf("[.] bytes object info\n");

	if (!PyBytes_Check(p))
	{
		printf("  [ERROR] Invalid Bytes Object\n");
		fflush(stdout);
		return;
	}

	size = bytes->ob_base.ob_size;
	printf("  size: %ld\n", size);
	printf("  trying string: %s\n", bytes->ob_sval);

	limit = size + 1;
	if (limit > 10)
		limit = 10;

	printf("  first %ld bytes: ", limit);
	for (i = 0; i < limit; i++)
	{
		printf("%02x", bytes->ob_sval[i] & 0xff);
		if (i < limit - 1)
			printf(" ");
	}
	printf("\n");
	fflush(stdout);
}

/**
 * format_python_float - Formats a double the way Python's repr() does:
 * shortest decimal string that round-trips to the same double, with a
 * trailing ".0" added if the result would otherwise look like an integer
 * @val: the double to format
 * @buf: destination buffer (must be at least 32 bytes)
 */
void format_python_float(double val, char *buf)
{
	int precision;
	double parsed;

	for (precision = 1; precision <= 17; precision++)
	{
		sprintf(buf, "%.*g", precision, val);
		parsed = strtod(buf, NULL);
		if (parsed == val)
			break;
	}

	if (strchr(buf, '.') == NULL && strchr(buf, 'e') == NULL &&
	    strchr(buf, 'n') == NULL && strchr(buf, 'N') == NULL)
		strcat(buf, ".0");
}

/**
 * print_python_float - Prints basic info about Python float objects
 * @p: PyObject float
 */
void print_python_float(PyObject *p)
{
	PyFloatObject *flt = (PyFloatObject *)p;
	char buf[32];

	printf("[.] float object info\n");

	if (!PyFloat_Check(p))
	{
		printf("  [ERROR] Invalid Float Object\n");
		fflush(stdout);
		return;
	}

	format_python_float(flt->ob_fval, buf);
	printf("  value: %s\n", buf);
	fflush(stdout);
}
