#ifndef HNM_H
#define HNM_H

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

/* main.c */
int process_file(const char *prog_name, const char *filename);

/* nm_64.c */
void parse_64(const char *prog_name, const char *filename, void *map,
	      size_t size, int need_swap);

/* nm_32.c */
void parse_32(const char *prog_name, const char *filename, void *map,
	      size_t size, int need_swap);

/* symbol_type.c */
char get_type_64(Elf64_Sym *sym, Elf64_Shdr *shdr);
char get_type_32(Elf32_Sym *sym, Elf32_Shdr *shdr);

/* endian.c */
uint16_t swap_16(uint16_t val);
uint32_t swap_32(uint32_t val);
uint64_t swap_64(uint64_t val);

#endif /* HNM_H */
