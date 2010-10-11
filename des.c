#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "des.h"

static unsigned char ip_first[] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};

static unsigned char ip_second[] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25

};

#define	BIT_POS(bit)	(7 - ((bit) % 8))
#define BYTE_POS(bit)	((bit) / 8)

static void des_ip_first(union block *current)
{
	union block prev;
	unsigned char swap;
	int i;

	memcpy(&prev, current, sizeof(prev));
	for (i = 0; i < 64; ++i) {
		/*
		 * swap the bit at position (ip_second[i] - 1) in prev with the bit at position i in block
		 * swap is set to a byte with every bit at 0 except the bit at position i, which is set to bit at (ip_second[i] - 1)
		 * then, the bit at position i in block is cleared, and its corresponding byte is AND with swap
		 */
		swap = prev.bytes[BYTE_POS(ip_first[i] - 1)] & (1 << (BIT_POS(ip_first[i] - 1)));
		swap >>= BIT_POS(ip_first[i] - 1);
		swap <<= BIT_POS(i);
		current->bytes[BYTE_POS(i)] &= ~(1 << BIT_POS(i));
		current->bytes[BYTE_POS(i)] |= swap;
	}
}

static void des_ip_second(union block *current)
{
	union block prev;
	unsigned char swap;
	int i;

	memcpy(&prev, current, sizeof(prev));
	for (i = 0; i < 64; ++i) {
		/*
		 * swap the bit at position (ip_second[i] - 1) in prev with the bit at position i in block
		 * swap is set to a byte with every bit at 0 except the bit at position i, which is set to bit at (ip_second[i] - 1)
		 * then, the bit at position i in block is cleared, and its corresponding byte is AND with swap
		 */
		swap = prev.bytes[BYTE_POS(ip_second[i] - 1)] & (1 << BIT_POS(ip_second[i] - 1));
		swap >>= BIT_POS(ip_second[i] - 1);
		swap <<= BIT_POS(i);
		current->bytes[BYTE_POS(i)] &= ~(1 << BIT_POS(i));
		current->bytes[BYTE_POS(i)] |= swap;
	}
	
}

void des_cipher_block(struct des *des, union block *block)
{
	printf("Initial BLOCK:\n");
	DUMP_BLOCK(block->bytes);
	des_ip_first(block);
	printf("BLOCK after ip_first:\n");
	DUMP_BLOCK(block->bytes);
	/* real stuff here */
	des_ip_second(block);
	printf("Block after ip_second:\n");
	DUMP_BLOCK(block->bytes);
	printf("\n\n");
}
