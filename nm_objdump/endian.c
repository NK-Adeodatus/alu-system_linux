#include "hnm.h"

/**
 * swap_16 - swap bytes of 16-bit unsigned int
 * @val: value to swap
 * Return: swapped value
 */
uint16_t swap_16(uint16_t val)
{
	return ((val << 8) | (val >> 8));
}

/**
 * swap_32 - swap bytes of 32-bit unsigned int
 * @val: value to swap
 * Return: swapped value
 */
uint32_t swap_32(uint32_t val)
{
	return (((val >> 24) & 0x000000ff) |
		((val << 8) & 0x00ff0000) |
		((val >> 8) & 0x0000ff00) |
		((val << 24) & 0xff000000));
}

/**
 * swap_64 - swap bytes of 64-bit unsigned int
 * @val: value to swap
 * Return: swapped value
 */
uint64_t swap_64(uint64_t val)
{
	return (((uint64_t)swap_32(val & 0xFFFFFFFF) << 32) |
		swap_32(val >> 32));
}
