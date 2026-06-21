#include "hnm.h"

/**
 * parse_32 - parse a 32-bit ELF file
 * @filename: name of the file
 * @map: pointer to the mapped file
 * @size: size of the file
 * @need_swap: whether byteswapping is needed
 */
void parse_32(const char *filename, void *map, size_t size, int need_swap)
{
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)map;
	Elf32_Shdr *shdr, *symtab = NULL, *strtab = NULL, *sec;
	Elf32_Sym *syms, temp_sym;
	char *str_table, t, *name;
	uint16_t i, shnum;
	uint32_t j, sym_cnt;
	Elf32_Shdr temp_shdr;

	(void)size;
	shdr = (Elf32_Shdr *)((char *)map + (need_swap ? swap_32(ehdr->e_shoff) : ehdr->e_shoff));
	shnum = need_swap ? swap_16(ehdr->e_shnum) : ehdr->e_shnum;

	for (i = 0; i < shnum; i++)
	{
		Elf32_Shdr *curr;
		uint32_t sh_type;

		curr = &shdr[i];
		sh_type = need_swap ? swap_32(curr->sh_type) : curr->sh_type;

		if (sh_type == SHT_SYMTAB)
		{
			symtab = curr;
			strtab = &shdr[need_swap ? swap_32(symtab->sh_link) : symtab->sh_link];
			break;
		}
	}

	if (!symtab)
	{
		fprintf(stderr, "%s: %s: no symbols\n", "hnm", filename);
		return;
	}

	syms = (Elf32_Sym *)((char *)map + (need_swap ? swap_32(symtab->sh_offset) : symtab->sh_offset));
	sym_cnt = (need_swap ? swap_32(symtab->sh_size) : symtab->sh_size) / sizeof(Elf32_Sym);
	str_table = (char *)map + (need_swap ? swap_32(strtab->sh_offset) : strtab->sh_offset);

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
