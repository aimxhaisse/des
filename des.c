#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

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

/*
 * First permutation
 */
static void des_ip_first(unsigned char *current)
{
	unsigned char prev[8];
	unsigned char swap;
	int i;

	memcpy(&prev, current, sizeof(prev));
	for (i = 0; i < 64; ++i) {
		swap = prev[BYTE_POS(ip_first[i] - 1)] & (1 << (BIT_POS(ip_first[i] - 1)));
		swap >>= BIT_POS(ip_first[i] - 1);
		swap <<= BIT_POS(i);
		current[BYTE_POS(i)] &= ~(1 << BIT_POS(i));
		current[BYTE_POS(i)] |= swap;
	}
}

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

/*
 * Second permutation
 */
static void des_ip_second(unsigned char *current)
{
	unsigned char prev[8];
	unsigned char swap;
	int i;

	memcpy(&prev, current, sizeof(prev));
	for (i = 0; i < 64; ++i) {
		swap = prev[BYTE_POS(ip_second[i] - 1)] & (1 << BIT_POS(ip_second[i] - 1));
		swap >>= BIT_POS(ip_second[i] - 1);
		swap <<= BIT_POS(i);
		current[BYTE_POS(i)] &= ~(1 << BIT_POS(i));
		current[BYTE_POS(i)] |= swap;
	}
}

static unsigned char exp_right[] = {
	32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1
};

/*
 * Block expansion
 */
static void des_exp(unsigned char block[4], unsigned char exp[6])
{
	unsigned char swap;
	int i;

	for (i = 0; i < 48; ++i) {
		swap = block[BYTE_POS(exp_right[i] - 1)] & (1 << BIT_POS(exp_right[i] - 1));
		swap >>= BIT_POS(exp_right[i] - 1);
		swap <<= BIT_POS(i);
		exp[BYTE_POS(i)] &= ~(1 << BIT_POS(i));
		exp[BYTE_POS(i)] |= swap;
	}
}

static unsigned char key_pc1[] = {
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4
};

/*
 * Key scheduling
 */
void des_key_permute(unsigned char *key)
{
	unsigned char tmp[8];
	unsigned char swap;
	unsigned int i;

	memcpy(tmp, key, sizeof(tmp));
	for (i = 0; i < 56; ++i) {
		swap = tmp[BYTE_POS(key_pc1[i] - 1)] & (1 << BIT_POS(key_pc1[i] - 1));
		swap >>= BIT_POS(key_pc1[i] - 1);
		swap <<= BIT_POS(i);
		key[BYTE_POS(i)] &= ~(1 << BIT_POS(i));
		key[BYTE_POS(i)] |= swap;
	}
}

static unsigned char subkey_pc2[] = {
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

/*
 * Subkey permutation
 */
static void des_subkey_permute(unsigned char *key, unsigned char *result)
{
	unsigned char swap;
	unsigned int i;

	for (i = 0; i < 56; ++i) {
		swap = key[BYTE_POS(subkey_pc2[i] - 1)] & (1 << BIT_POS(subkey_pc2[i] - 1));
		swap >>= BIT_POS(subkey_pc2[i] - 1);
		swap <<= BIT_POS(i);
		result[BYTE_POS(i)] &= ~(1 << BIT_POS(i));
		result[BYTE_POS(i)] |= swap;
	}
}

static unsigned char subkeys_rotate[] = {
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

void des_generate_subkeys(unsigned char *key, unsigned char **subkeys)
{
	int i;
	unsigned int l, r;

	for (i = 0; i < 16; ++i) {
		DUMP_KEY(key);
		BLOCK_TO_INT(key, l, r);

		/* rotate subkeys according to the round */
		l >>= subkeys_rotate[i];
		r >>= subkeys_rotate[i];

		/* ugly, but we need to merge the common byte */
		key[0] = ((unsigned char *) &l)[0];
		key[1] = ((unsigned char *) &l)[1];
		key[2] = ((unsigned char *) &l)[2];
		key[3] = (((unsigned char *) &l)[3] & 0xF0) | (((unsigned char *) &r)[0] & 0x0F);
		key[4] = ((unsigned char *) &r)[1];
		key[5] = ((unsigned char *) &r)[2];
		key[6] = ((unsigned char *) &r)[3];

		/* do the permutation and store it to the subkey */
		des_subkey_permute(key, subkeys[i]);

		break;
	}
}

void des_cipher_block(unsigned char *block)
{
	unsigned char right[6];

	des_ip_first(block);
	memcpy(right, &block[4], 4 * sizeof(unsigned char));
	des_exp(block, right);
	des_ip_second(block);
	exit(1);
}
