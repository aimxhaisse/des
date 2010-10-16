#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>

#include "des.h"

void usage(void);

/* default settings */
static struct des des = {
	.op =	ENCRYPT,
	.mode = EBC,
	.ifd =	-1,
	.ofd =	-1
};

/* check validity of parameters (files, ...) exit on failure */
static void des_init(void)
{
	if (des.ipath == NULL || des.opath == NULL) {
		usage();
		/* NOTREACHED */
	}
	if ((des.ifd = open(des.ipath, O_RDONLY)) == -1)
		err(1, "can't open input file %s", des.ipath);
	if ((des.ofd = open(des.opath, O_WRONLY | O_TRUNC | O_CREAT, 0644)) == -1)
		err(1, "can't open output file %s", des.opath);
}

int main(int argc, char *argv[])
{
	int ch;

	while ((ch = getopt(argc, argv, "dei:o:k:")) != -1)
		switch (ch) {
		case 'd':
			des.op = DECRYPT;
			break;
		case 'e':
			des.op = ENCRYPT;
			break;
		case 'i':
			des.ipath = optarg;
			break;
		case 'o':
			des.opath = optarg;
			break;
		case 'k':
			memcpy(des.key, optarg, MIN(strlen(optarg), 8));
			break;
		default:
			usage();
			/* NOTREACHED */
		}

	des_init();
	des_key_permute(des.key);
	des_generate_subkeys(des.key, des.subkeys);
	des_mode(&des);

	return 0;
}

void usage(void)
{
	fprintf(stderr, "usage: des [-de] [-i infile] [-o outfile] [-k=key]\n"
	    "-e\t encrypt infile to outfile using key\n"
	    "-d\t decrypt infile to outfile using key\n");
	exit(1);
}
