#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "des.h"

static void des_mode_ebc(struct des * des)
{
	unsigned char block[8];
	int nb_read, bytes, pad, i;

	do {
		/* extract 8 bytes from the file (slightly complicated but deal with interruptions like signals) */
		bytes = read(des->ifd, &block, sizeof(block));
		nb_read = bytes;
		while (bytes > 0 && nb_read < (int) sizeof(block)) {
			bytes = read(des->ifd, &block[bytes], sizeof(block) - bytes);
			if (bytes > 0)
				nb_read += bytes;
		}
		/* create a block (pad it if necessary) */
		if (nb_read > 0) {
			pad = 8 - nb_read;
			for (i = pad; i > 0; --i) {
				block[8 - i] = pad;
			}
			des_cipher_block(des, block);
			write(des->ofd, block, sizeof(block));
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
