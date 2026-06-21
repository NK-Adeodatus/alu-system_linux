#include "hnm.h"

/**
 * is_little_endian - check host endianness
 * Return: 1 if little endian, 0 otherwise
 */
static int is_little_endian(void)
{
	int i = 1;

	return (*(char *)&i == 1);
}

/**
 * process_file - process an ELF file
 * @filename: name of the file
 * Return: 0 on success, 1 on error
 */
int process_file(const char *filename)
{
	int fd, need_swap = 0;
	struct stat sb;
	void *map;
	unsigned char *e_ident;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "%s: '%s': No such file\n", "hnm", filename);
		return (1);
	}
	if (fstat(fd, &sb) == -1 || sb.st_size < EI_NIDENT)
	{
		close(fd);
		return (1);
	}
	map = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED)
	{
		close(fd);
		return (1);
	}
	e_ident = (unsigned char *)map;
	if (e_ident[EI_MAG0] != ELFMAG0 || e_ident[EI_MAG1] != ELFMAG1 ||
	    e_ident[EI_MAG2] != ELFMAG2 || e_ident[EI_MAG3] != ELFMAG3)
	{
		fprintf(stderr, "%s: %s: file format not recognized\n", "hnm", filename);
		munmap(map, sb.st_size);
		close(fd);
		return (1);
	}
	if ((e_ident[EI_DATA] == ELFDATA2MSB && is_little_endian()) ||
	    (e_ident[EI_DATA] == ELFDATA2LSB && !is_little_endian()))
		need_swap = 1;

	if (e_ident[EI_CLASS] == ELFCLASS32)
		parse_32(filename, map, sb.st_size, need_swap);
	else if (e_ident[EI_CLASS] == ELFCLASS64)
		parse_64(filename, map, sb.st_size, need_swap);

	munmap(map, sb.st_size);
	close(fd);
	return (0);
}

/**
 * main - entry point
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
	int i, ret = 0;

	if (argc == 1)
		return (process_file("a.out"));

	for (i = 1; i < argc; i++)
	{
		if (argc > 2)
			printf("\n%s:\n", argv[i]);
		ret |= process_file(argv[i]);
	}
	return (ret);
}
