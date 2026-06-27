#include <Python.h>
#include <listobject.h>

/**
 * print_python_list - Prints basic info about Python lists
 * @p: PyObject list
 */
void print_python_list(PyObject *p)
{
	int size, alloc, i;
	PyListObject *list = (PyListObject *)p;
	PyObject *item;

	size = Py_SIZE(p);
	alloc = list->allocated;

	printf("[*] Python list info\n");
	printf("[*] Size of the Python List = %d\n", size);
	printf("[*] Allocated = %d\n", alloc);

	for (i = 0; i < size; i++)
	{
		item = PyList_GetItem(p, i);
		printf("Element %d: %s\n", i, item->ob_type->tp_name);
	}
}
