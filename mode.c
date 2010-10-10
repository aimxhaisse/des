#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "des.h"

static void des_mode_ebc(struct des * des)
{
	union block input_block;
	union block output_block;
	int nb_read, bytes, pad, i;

	do {
		/* extract 8 bytes from the file (slightly complicated but deal with interruptions like signals) */
		bytes = read(des->ifd, &input_block, sizeof(input_block));
		nb_read = bytes;
		while (bytes > 0 && nb_read < sizeof(input_block)) {
			bytes = read(des->ifd, &input_block.bytes[bytes], sizeof(input_block) - bytes);
			if (bytes > 0)
				nb_read += bytes;
		}
		/* create a block (pad it if necessary) */
		if (nb_read > 0) {
			pad = 8 - nb_read;
			for (i = pad; i > 0; --i) {
				input_block.bytes[8 - i] = pad;
			}
			des_cipher_block(&input_block);
			/* Write output block to output file */
		}
	} while (bytes > 0);
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
