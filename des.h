#ifndef DES_H
#define DES_H

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

enum operation {
	DECRYPT, /* decrypt input file to output file using key */
	ENCRYPT	 /* encrypt input file to output file using key */
};

enum ciphermode {
	EBC /* electronic codebook */
};

union block {
	unsigned char	bytes[8];
	unsigned int	half[2];
};

struct des {
	enum operation	op;	/* current operation */
	enum ciphermode	mode;	/* current mode */
	char		key[8];	/* key to use */
	char *		ipath;	/* path of input file */
	char *		opath;	/* path of output file */
	int		ifd;	/* input file descriptor */
	int		ofd;	/* output file descriptor */
};

void des_mode(struct des *des);
void des_cipher_block(union block *block);

#define DUMP_BLOCK(bytes)						\
	fprintf(stderr,							\
	    "0x%02X 0x%02X 0x%02X 0x%02X "				\
	    "0x%02X 0x%02X 0x%02X 0x%02X\n",				\
	    bytes[0],							\
	    bytes[1],							\
	    bytes[2],							\
	    bytes[3],							\
	    bytes[4],							\
	    bytes[5],							\
	    bytes[6],							\
	    bytes[7])

#endif /* DES_H */
