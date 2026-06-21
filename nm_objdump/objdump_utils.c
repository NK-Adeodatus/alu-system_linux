#include "hobjdump.h"

#define P_FLAG(flag, name) \
	do { \
		if (flags & (flag)) \
		{ \
			printf("%s%s", first ? "" : ", ", name); \
			first = 0; \
		} \
	} while (0)

/**
 * print_flags - prints BFD flags
 * @flags: BFD flags
 */
void print_flags(uint32_t flags)
{
	int first = 1;

	if (flags == 0)
	{
		printf("BFD_NO_FLAGS\n");
		return;
	}
	P_FLAG(HAS_RELOC, "HAS_RELOC");
	P_FLAG(EXEC_P, "EXEC_P");
	P_FLAG(HAS_LINENO, "HAS_LINENO");
	P_FLAG(HAS_DEBUG, "HAS_DEBUG");
	P_FLAG(HAS_SYMS, "HAS_SYMS");
	P_FLAG(HAS_LOCALS, "HAS_LOCALS");
	P_FLAG(DYNAMIC, "DYNAMIC");
	P_FLAG(WP_TEXT, "WP_TEXT");
	P_FLAG(D_PAGED, "D_PAGED");
	printf("\n");
}

/**
 * print_hexdump - prints a section's contents in hex and ascii
 * @data: pointer to section data
 * @size: size of section
 * @vma: virtual memory address
 */
void print_hexdump(const unsigned char *data, uint64_t size, uint64_t vma)
{
	uint64_t i, j, max_vma = vma + size - 1;
	int is_print, vma_len = 4;

	while (max_vma > 0xFFFF && max_vma >> (vma_len * 4))
		vma_len++;

	for (i = 0; i < size; i += 16)
	{
		printf(" %0*lx ", vma_len, vma + i);
		for (j = 0; j < 16; j++)
		{
			if (i + j < size)
				printf("%02x", data[i + j]);
			else
				printf("  ");
			if ((j % 4) == 3)
				printf(" ");
		}
		printf(" ");
		for (j = 0; j < 16; j++)
		{
			if (i + j < size)
			{
				is_print = (data[i + j] >= 32 && data[i + j] <= 126);
				printf("%c", is_print ? data[i + j] : '.');
			}
			else
				printf(" ");
		}
		printf("\n");
	}
}
