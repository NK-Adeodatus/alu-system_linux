#include <Python.h>
#include <listobject.h>
#include <bytesobject.h>
#include <string.h>

void print_python_list(PyObject *p);
void print_python_bytes(PyObject *p);

/**
 * print_python_list - Prints basic info about Python lists
 * @p: PyObject list
 */
void print_python_list(PyObject *p)
{
	Py_ssize_t size, alloc, i;
	PyListObject *list = (PyListObject *)p;
	PyObject *item;

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
	}
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
}
