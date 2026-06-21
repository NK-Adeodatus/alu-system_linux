#include "hobjdump.h"

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
 * check_elf - validates and processes ELF format
 * @prog: program name
 * @fname: file name
 * @map: mapped memory
 * @sb: stat structure
 * Return: 0 on success, 1 on failure
 */
static int check_elf(const char *prog, const char *fname, void *map,
		     struct stat *sb)
{
	unsigned char *e_ident = (unsigned char *)map;
	int need_swap = 0;

	if (e_ident[EI_MAG0] != ELFMAG0 || e_ident[EI_MAG1] != ELFMAG1 ||
	    e_ident[EI_MAG2] != ELFMAG2 || e_ident[EI_MAG3] != ELFMAG3)
	{
		fprintf(stderr, "%s: %s: File format not recognized\n", prog, fname);
		munmap(map, sb->st_size);
		return (1);
	}

	if ((e_ident[EI_DATA] == ELFDATA2MSB && is_little_endian()) ||
	    (e_ident[EI_DATA] == ELFDATA2LSB && !is_little_endian()))
		need_swap = 1;

	if (e_ident[EI_CLASS] == ELFCLASS32)
		print_objdump_32(fname, map, sb->st_size, need_swap);
	else if (e_ident[EI_CLASS] == ELFCLASS64)
		print_objdump_64(fname, map, sb->st_size, need_swap);

	munmap(map, sb->st_size);
	return (0);
}

/**
 * process_file - process an ELF file
 * @prog_name: program name
 * @filename: name of the file
 * Return: 0 on success, 1 on error
 */
int process_file(const char *prog_name, const char *filename)
{
	int fd;
	struct stat sb;
	void *map;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "%s: '%s': No such file\n", prog_name, filename);
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
	close(fd);

	return (check_elf(prog_name, filename, map, &sb));
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
		return (process_file(argv[0], "a.out"));

	for (i = 1; i < argc; i++)
	{
		ret |= process_file(argv[0], argv[i]);
	}
	return (ret);
}
