#include <Python.h>
#include <stdio.h>

void print_python_string(PyObject *p);
void print_utf8_codepoint(unsigned int cp);

/**
 * print_utf8_codepoint - Encodes and prints a single Unicode code point
 * as UTF-8 bytes
 * @cp: the Unicode code point to encode
 */
void print_utf8_codepoint(unsigned int cp)
{
	if (cp < 0x80)
	{
		putchar(cp);
	}
	else if (cp < 0x800)
	{
		putchar(0xC0 | (cp >> 6));
		putchar(0x80 | (cp & 0x3F));
	}
	else if (cp < 0x10000)
	{
		putchar(0xE0 | (cp >> 12));
		putchar(0x80 | ((cp >> 6) & 0x3F));
		putchar(0x80 | (cp & 0x3F));
	}
	else
	{
		putchar(0xF0 | (cp >> 18));
		putchar(0x80 | ((cp >> 12) & 0x3F));
		putchar(0x80 | ((cp >> 6) & 0x3F));
		putchar(0x80 | (cp & 0x3F));
	}
}

/**
 * print_python_string - Prints basic info about Python string objects
 * @p: PyObject string
 */
void print_python_string(PyObject *p)
{
	Py_ssize_t length, i;
	int kind;
	void *data;

	printf("[.] string object info\n");

	if (!PyUnicode_Check(p))
	{
		printf("  [ERROR] Invalid String Object\n");
		fflush(stdout);
		return;
	}

	length = PyUnicode_GET_LENGTH(p);
	kind = PyUnicode_KIND(p);
	data = PyUnicode_DATA(p);

	if (PyUnicode_IS_COMPACT_ASCII(p))
		printf("  type: compact ascii\n");
	else if (PyUnicode_IS_COMPACT(p))
		printf("  type: compact unicode object\n");
	else
		printf("  type: legacy string object\n");

	printf("  length: %ld\n", length);
	printf("  value: ");
	for (i = 0; i < length; i++)
		print_utf8_codepoint(PyUnicode_READ(kind, data, i));
	printf("\n");
	fflush(stdout);
}
