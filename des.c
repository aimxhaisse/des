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
		/* swap the two bits using the matrice ip_first */
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
		/* swap the two bits using the matrice ip_second */
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
static void des_exp(unsigned char *block, unsigned char *exp)
{
	unsigned char swap;
	int i;

	for (i = 0; i < 48; ++i) {
		/* swap the two bits using the matrice exp_right */
		swap = block[BYTE_POS(exp_right[i] - 1)] & (1 << BIT_POS(exp_right[i] - 1));
		swap >>= BIT_POS(exp_right[i] - 1);
		swap <<= BIT_POS(i);
		exp[BYTE_POS(i)] &= ~(1 << BIT_POS(i));
		exp[BYTE_POS(i)] |= swap;
	}
}

static unsigned char p[] = {
	16, 7, 20, 21, 29, 12, 28, 17,
	1, 15, 23, 26, 5, 18, 31, 10,
	2, 8, 24, 14, 32, 27, 3, 9,
	19, 13, 30, 6, 22, 11, 4, 25
};

static void des_p(unsigned char * s)
{
	unsigned char swap, tmp[8];
	int i;

	memcpy(tmp, s, sizeof(tmp));
	for (i = 0; i < 32; ++i) {
		/* swap the two bits using the matrice p */
		swap = tmp[BYTE_POS(p[i] - 1)] & (1 << BIT_POS(p[i] - 1));
		swap >>= BIT_POS(p[i] - 1);
		swap <<= BIT_POS(i);
		s[BYTE_POS(i)] &= ~(1 << BIT_POS(i));
		s[BYTE_POS(i)] |= swap;
	}
}

static unsigned char sboxes[8][4][16] = {

	{	/* s-box 1 */
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	},

	{	/* s-box 2 */
		{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
		{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
		{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
		{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
	},

	{	/* s-box 3 */
		{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
		{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
		{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
		{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
	},

	{	/* s-box 4 */
		{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
		{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
		{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
		{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
	},


	{	/* s-box 5 */
		{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
		{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
		{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
		{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
	},


	{	/* s-box 6 */
		{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
		{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
		{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
		{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
	},

	{	/* s-box 7 */
		{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
		{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
		{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
		{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
	},

	{	/* s-box 8 */
		{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
		{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
		{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
		{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
	}
	/*
	 * I wish I knew emacs lisp ;) wasted 30 minuts copy/pasting, adding commas ...
	 */
};

/*
 * Main entry
 */
void des_cipher_block(struct des *des, unsigned char *block)
{
	int i, j;
	unsigned char left[4], right[6], tmp[6], b[8], s[4], prev[6];
	unsigned char row, col;

	des_ip_first(block);
	memcpy(right, &block[4], 4 * sizeof(unsigned char));
	memcpy(left, &block, 4 * sizeof(unsigned char));

	for (i = 0; i < 16; ++i) {

		memcpy(prev, right, 4 * sizeof(unsigned char));
		des_exp(prev, right);
		memcpy(tmp, right, sizeof(tmp));

		for (j = 0; j < 6; ++j)
			tmp[j] = right[j] ^ des->subkeys[i][j];

		/* each rank in b is made of 6 linear bits from tmp */
		b[0] = (tmp[0] & 0xFC) >> 2;
		b[1] = ((tmp[0] & 0x03) << 4) | (tmp[1] & 0xF0);
		b[2] = ((tmp[1] & 0x0F) << 2) | (tmp[2] & 0xC0);
		b[3] = tmp[2] & 0x3F;
		b[4] = (tmp[3] & 0xFC) >> 2;
		b[5] = ((tmp[3] & 0x03) << 4) | (tmp[4] & 0xF0);
		b[6] = ((tmp[4] & 0x0F) << 2) | (tmp[5] & 0xC0);
		b[7] = tmp[6] & 0x3F;

		memset(s, 0, sizeof(s));
		for (j = 0; j < 8; ++j) {
			row = (b[j] & 0x01) | ((b[j] & 0x20) >> 4);
			col = (b[j] & 0x1E) >> 1;
			s[j / 2] |= sboxes[j][row][col] << (!(j % 2) ? 4 : 0);
		}

		des_p(s);

		s[0] ^= left[0];
		s[1] ^= left[1];
		s[2] ^= left[2];
		s[3] ^= left[3];

		memcpy(left, right, sizeof(left));
		memcpy(right, s, sizeof(s));
	}

	memcpy(block, right, 4 * sizeof(unsigned char));
	memcpy(block + 4, left, 4 * sizeof(unsigned char));

	des_ip_second(block);
}
