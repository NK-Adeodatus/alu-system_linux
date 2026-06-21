#include "hnm.h"

/**
 * get_type_general - determine symbol type character
 * @bind: symbol binding
 * @type: symbol type
 * @shndx: section header index
 * @sh_type: section type
 * @sh_flags: section flags
 * Return: character representing the symbol type
 */
static char get_type_general(uint8_t bind, uint8_t type, uint16_t shndx,
			     uint32_t sh_type, uint64_t sh_flags)
{
	char c = '?';

	if (bind == STB_WEAK)
	{
		if (type == STT_OBJECT)
			return ((shndx == SHN_UNDEF) ? 'v' : 'V');
		return ((shndx == SHN_UNDEF) ? 'w' : 'W');
	}
	if (shndx == SHN_UNDEF)
		c = 'U';
	else if (shndx == SHN_ABS)
		c = 'A';
	else if (shndx == SHN_COMMON)
		c = 'C';
	else
	{
		if (sh_type == SHT_NOBITS)
			c = 'B';
		else if (sh_flags & SHF_EXECINSTR)
			c = 'T';
		else if (sh_type == SHT_INIT_ARRAY || sh_type == SHT_FINI_ARRAY ||
			 sh_type == SHT_PREINIT_ARRAY)
			c = 'T';
		else if ((sh_flags & SHF_ALLOC) && !(sh_flags & SHF_WRITE))
			c = 'R';
		else if (sh_flags & SHF_WRITE)
			c = 'D';
		else
			c = 't';
	}

	if (bind == STB_LOCAL && c >= 'A' && c <= 'Z')
		c += 32;

	return (c);
}

/**
 * get_type_64 - determine symbol type character for 64-bit ELF
 * @sym: pointer to symbol
 * @shdr: pointer to section header
 * Return: character representing the symbol type
 */
char get_type_64(Elf64_Sym *sym, Elf64_Shdr *shdr)
{
	uint8_t bind = ELF64_ST_BIND(sym->st_info);
	uint8_t type = ELF64_ST_TYPE(sym->st_info);
	uint16_t shndx = sym->st_shndx;
	uint32_t sh_type = shdr ? shdr->sh_type : 0;
	uint64_t sh_flags = shdr ? shdr->sh_flags : 0;

	return (get_type_general(bind, type, shndx, sh_type, sh_flags));
}

/**
 * get_type_32 - determine symbol type character for 32-bit ELF
 * @sym: pointer to symbol
 * @shdr: pointer to section header
 * Return: character representing the symbol type
 */
char get_type_32(Elf32_Sym *sym, Elf32_Shdr *shdr)
{
	uint8_t bind = ELF32_ST_BIND(sym->st_info);
	uint8_t type = ELF32_ST_TYPE(sym->st_info);
	uint16_t shndx = sym->st_shndx;
	uint32_t sh_type = shdr ? shdr->sh_type : 0;
	uint64_t sh_flags = shdr ? shdr->sh_flags : 0;

	return (get_type_general(bind, type, shndx, sh_type, sh_flags));
}
