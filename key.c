#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "des.h"

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
		/* swap the two bits using the matrice key_pc1 */
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
		/* swap the two bits using the matrice subkey_pc2 */
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

/*
 * subkey generation
 */
void des_generate_subkeys(unsigned char *key, unsigned char (*subkeys)[7])
{
	int i;
	unsigned char l[4], r[4];

	for (i = 0; i < 16; ++i) {

		if (subkeys_rotate[i] == 1) {
			l[0] = (key[0] << 1) | ((key[1] & 0x80) >> 7);
			l[1] = (key[1] << 1) | ((key[2] & 0x80) >> 7);
			l[2] = (key[2] << 1) | ((key[3] & 0x80) >> 7);
			l[3] = ((key[3] & 0xF0) << 1) | ((key[0] & 0x80) >> 3);

			r[0] = (key[3] << 1) | ((key[4] & 0x80) >> 7);
			r[1] = (key[4] << 1) | ((key[5] & 0x80) >> 7);
			r[2] = (key[5] << 1) | ((key[6] & 0x80) >> 7);
			r[3] = (key[6] << 1) | ((key[3] & 0x08) >> 3);

		} else {
			l[0] = (key[0] << 2) | ((key[1] & 0xC0) >> 6);
			l[1] = (key[1] << 2) | ((key[2] & 0xC0) >> 6);
			l[2] = (key[2] << 2) | ((key[3] & 0xC0) >> 6);
			l[3] = ((key[3] & 0xF0) << 2) | ((key[0] & 0xC0) >> 2);

			r[0] = (key[3] << 2) | ((key[4] & 0xC0) >> 6);
			r[1] = (key[4] << 2) | ((key[5] & 0xC0) >> 6);
			r[2] = (key[5] << 2) | ((key[6] & 0xC0) >> 6);
			r[3] = (key[6] << 2) | ((key[3] & 0x0C) >> 2);
		}

		/* ugly, but we need to merge the common byte */
		key[0] = l[0];
		key[1] = l[1];
		key[2] = l[2];
		key[3] = (l[3] & 0xF0) | (r[0] & 0x0F);
		key[4] = r[1];
		key[5] = r[2];
		key[6] = r[3];

		/* do the permutation and store it to the subkey */
		des_subkey_permute(key, subkeys[i]);
	}
}
