#include "hnm.h"

/**
 * print_syms_32 - prints symbols for 32-bit ELF
 * @syms: pointer to symbols
 * @sym_cnt: number of symbols
 * @str_table: string table
 * @shdr: section headers
 * @shnum: number of section headers
 * @need_swap: endian swap flag
 */
static void print_syms_32(Elf32_Sym *syms, uint32_t sym_cnt, char *str_table,
			  Elf32_Shdr *shdr, uint16_t shnum, int need_swap)
{
	uint32_t j;
	Elf32_Sym temp_sym;
	Elf32_Shdr temp_shdr, *sec;
	char *name, t;

	for (j = 0; j < sym_cnt; j++)
	{
		temp_sym = syms[j];
		if (need_swap)
		{
			temp_sym.st_name = swap_32(temp_sym.st_name);
			temp_sym.st_shndx = swap_16(temp_sym.st_shndx);
			temp_sym.st_value = swap_32(temp_sym.st_value);
		}
		if (temp_sym.st_name == 0 || ELF32_ST_TYPE(temp_sym.st_info) == STT_FILE ||
		    ELF32_ST_TYPE(temp_sym.st_info) == STT_SECTION)
			continue;

		name = str_table + temp_sym.st_name;
		sec = NULL;
		if (temp_sym.st_shndx != SHN_UNDEF && temp_sym.st_shndx < shnum)
		{
			temp_shdr = shdr[temp_sym.st_shndx];
			if (need_swap)
			{
				temp_shdr.sh_type = swap_32(temp_shdr.sh_type);
				temp_shdr.sh_flags = swap_32(temp_shdr.sh_flags);
			}
			sec = &temp_shdr;
		}
		t = get_type_32(&temp_sym, sec);
		if (temp_sym.st_shndx == SHN_UNDEF)
			printf("%8s %c %s\n", "", t, name);
		else
			printf("%08x %c %s\n", temp_sym.st_value, t, name);
	}
}

/**
 * parse_32 - parse a 32-bit ELF file
 * @prog: program name
 * @filename: name of the file
 * @map: pointer to the mapped file
 * @size: size of the file
 * @need_swap: whether byteswapping is needed
 */
void parse_32(const char *prog, const char *filename, void *map,
	      size_t size, int need_swap)
{
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)map;
	Elf32_Shdr *shdr, *symtab = NULL, *strtab = NULL;
	Elf32_Sym *syms;
	char *str_table;
	uint16_t i, shnum;
	uint32_t sym_cnt;

	(void)size;
	shdr = (Elf32_Shdr *)((char *)map +
			      (need_swap ? swap_32(ehdr->e_shoff) : ehdr->e_shoff));
	shnum = need_swap ? swap_16(ehdr->e_shnum) : ehdr->e_shnum;

	for (i = 0; i < shnum; i++)
	{
		Elf32_Shdr *curr = &shdr[i];
		uint32_t sh_type = need_swap ? swap_32(curr->sh_type) : curr->sh_type;

		if (sh_type == SHT_SYMTAB)
		{
			symtab = curr;
			strtab = &shdr[need_swap ? swap_32(symtab->sh_link) : symtab->sh_link];
			break;
		}
	}

	if (!symtab)
	{
		fprintf(stderr, "%s: %s: no symbols\n", prog, filename);
		return;
	}

	syms = (Elf32_Sym *)((char *)map +
			     (need_swap ? swap_32(symtab->sh_offset) : symtab->sh_offset));
	sym_cnt = (need_swap ? swap_32(symtab->sh_size) : symtab->sh_size) /
		  sizeof(Elf32_Sym);
	str_table = (char *)map +
		    (need_swap ? swap_32(strtab->sh_offset) : strtab->sh_offset);

	print_syms_32(syms, sym_cnt, str_table, shdr, shnum, need_swap);
}
