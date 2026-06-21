#include "hobjdump.h"

/**
 * get_arch_32 - get architecture string
 * @machine: machine type
 * Return: architecture string
 */
static const char *get_arch_32(uint16_t machine)
{
	if (machine == EM_386)
		return ("i386");
	return ("UNKNOWN!");
}

/**
 * get_format_32 - get file format string
 * @machine: machine type
 * @data: endianness (EI_DATA)
 * Return: format string
 */
static const char *get_format_32(uint16_t machine, unsigned char data)
{
	if (machine == EM_386)
		return ("elf32-i386");
	if (data == ELFDATA2MSB)
		return ("elf32-big");
	return ("elf32-little");
}

/**
 * print_sections_32 - print sections of 32-bit ELF
 * @map: memory map
 * @shdr: section headers
 * @shnum: number of section headers
 * @str_table: string table
 * @need_swap: endian swap flag
 * @elf_type: ELF file type
 */
static void print_sections_32(void *map, Elf32_Shdr *shdr, uint16_t shnum,
			      char *str_table, int need_swap, uint16_t elf_type)
{
	uint16_t i;
	char *name;

	for (i = 0; i < shnum; i++)
	{
		uint32_t s_type, s_size, s_addr, s_off, s_name;

		s_type = need_swap ? swap_32(shdr[i].sh_type) : shdr[i].sh_type;
		s_size = need_swap ? swap_32(shdr[i].sh_size) : shdr[i].sh_size;
		s_addr = need_swap ? swap_32(shdr[i].sh_addr) : shdr[i].sh_addr;
		s_off = need_swap ? swap_32(shdr[i].sh_offset) : shdr[i].sh_offset;
		s_name = need_swap ? swap_32(shdr[i].sh_name) : shdr[i].sh_name;

		if (s_size == 0 || s_type == SHT_NOBITS)
			continue;
		if (elf_type == ET_REL &&
		    (s_type == SHT_RELA || s_type == SHT_REL))
			continue;

		name = str_table + s_name;
		if (!strcmp(name, ".shstrtab") || !strcmp(name, ".symtab") ||
		    !strcmp(name, ".strtab"))
			continue;

		printf("Contents of section %s:\n", name);
		print_hexdump((unsigned char *)map + s_off, s_size, s_addr);
	}
}

/**
 * get_flags_32 - calculate BFD flags
 * @ehdr: ELF header
 * @shdr: section headers
 * @shnum: number of sections
 * @need_swap: endian swap flag
 * Return: BFD flags
 */
static uint32_t get_flags_32(Elf32_Ehdr *ehdr, Elf32_Shdr *shdr,
			     uint16_t shnum, int need_swap)
{
	uint32_t flags = BFD_NO_FLAGS;
	uint16_t type = need_swap ? swap_16(ehdr->e_type) : ehdr->e_type;
	uint16_t i;

	if (type == ET_EXEC)
		flags |= (EXEC_P | D_PAGED);
	else if (type == ET_DYN)
		flags |= (DYNAMIC | D_PAGED);
	else if (type == ET_REL)
		flags |= HAS_RELOC;

	for (i = 0; i < shnum; i++)
	{
		uint32_t sh_type;

		sh_type = need_swap ? swap_32(shdr[i].sh_type) : shdr[i].sh_type;
		if (sh_type == SHT_SYMTAB || sh_type == SHT_DYNSYM)
			flags |= HAS_SYMS;
	}
	return (flags);
}

/**
 * print_objdump_32 - process and print 32-bit ELF sections
 * @filename: file name
 * @map: memory map
 * @size: file size
 * @need_swap: endian swap flag
 */
void print_objdump_32(const char *filename, void *map,
		      size_t size, int need_swap)
{
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)map;
	Elf32_Shdr *shdr, *strtab;
	char *str_table;
	uint32_t flags = BFD_NO_FLAGS;
	uint16_t mach = need_swap ? swap_16(ehdr->e_machine) : ehdr->e_machine;
	uint16_t type = need_swap ? swap_16(ehdr->e_type) : ehdr->e_type;
	uint16_t shnum = need_swap ? swap_16(ehdr->e_shnum) : ehdr->e_shnum;
	uint32_t shoff = need_swap ? swap_32(ehdr->e_shoff) : ehdr->e_shoff;
	uint16_t shstrndx = need_swap ?
		swap_16(ehdr->e_shstrndx) : ehdr->e_shstrndx;
	uint32_t entry = need_swap ? swap_32(ehdr->e_entry) : ehdr->e_entry;

	(void)size;
	shdr = (Elf32_Shdr *)((char *)map + shoff);
	strtab = &shdr[shstrndx];
	str_table = (char *)map + (need_swap ? swap_32(strtab->sh_offset) :
				   strtab->sh_offset);

	flags = get_flags_32(ehdr, shdr, shnum, need_swap);

	printf("\n%s:     file format %s\n", filename,
	       get_format_32(mach, ehdr->e_ident[EI_DATA]));
	printf("architecture: %s, flags 0x%08x:\n", get_arch_32(mach), flags);
	print_flags(flags);
	printf("start address 0x%08x\n\n", entry);

	print_sections_32(map, shdr, shnum, str_table, need_swap, type);
}
