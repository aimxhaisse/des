#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "des.h"

static void des_mode_ebc(struct des * des)
{
	union block input_block;
	union block output_block;
	int nb_read, pad, i;

	do {
		nb_read = read(des->ifd, &input_block, sizeof(input_block));
		if (nb_read > 0) {
			pad = 8 - nb_read;
			for (i = pad; i > 0; --i) {
				input_block.bytes[8 - i] = pad;
			}
			DUMP_BLOCK(input_block.bytes);
			des_cipher_block(&input_block);
			/* Call encryption/decryption algorithm */
			/* Write output block to output file */
		}
	} while (nb_read > 0);
}

void des_mode(struct des * des)
{
	switch (des->mode) {
	case EBC:
		des_mode_ebc(des);
		break;
	default:
		fprintf(stderr, "des: unknown mode used\n");
		exit(1);
	}
}
