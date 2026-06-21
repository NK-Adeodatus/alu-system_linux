#ifndef HOBJDUMP_H
#define HOBJDUMP_H

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

/* BFD Flags */
#define BFD_NO_FLAGS 0x00
#define HAS_RELOC 0x01
#define EXEC_P 0x02
#define HAS_LINENO 0x04
#define HAS_DEBUG 0x08
#define HAS_SYMS 0x10
#define HAS_LOCALS 0x20
#define DYNAMIC 0x40
#define WP_TEXT 0x80
#define D_PAGED 0x100

/* endian.c */
uint16_t swap_16(uint16_t val);
uint32_t swap_32(uint32_t val);
uint64_t swap_64(uint64_t val);

/* objdump_utils.c */
void print_flags(uint32_t flags);
void print_hexdump(const unsigned char *data, uint64_t size, uint64_t vma);

/* objdump_64.c */
void print_objdump_64(const char *filename, void *map, size_t size, int need_swap);

/* objdump_32.c */
void print_objdump_32(const char *filename, void *map, size_t size, int need_swap);

#endif /* HOBJDUMP_H */
