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

struct des {
	enum operation	op;	/* current operation */
	enum ciphermode	mode;	/* current mode */
	unsigned char	key[8];	/* key to use */
	unsigned char	subkeys[16][7];
	char *		ipath;	/* path of input file */
	char *		opath;	/* path of output file */
	int		ifd;	/* input file descriptor */
	int		ofd;	/* output file descriptor */
};

void des_mode(struct des *des);
void des_cipher_block(struct des *des, unsigned char *block);
void binary_dumpc(unsigned char c);
void binary_dumpi(unsigned int c);
void des_key_permute(unsigned char *key);
void des_generate_subkeys(unsigned char *key, unsigned char **subkeys);

#define DUMP_KEY(key)				\
	binary_dumpc(key[0]);			\
	binary_dumpc(key[1]);			\
	binary_dumpc(key[2]);			\
	binary_dumpc(key[3]);			\
	binary_dumpc(key[4]);			\
	binary_dumpc(key[5]);			\
	binary_dumpc(key[6]);			\
	binary_dumpc(key[7]);			\
	printf("\n");

#define DUMP_BLOCK(bytes)				\
	fprintf(stderr,					\
		"0x%6X 0x%6X 0x%6X 0x%6X "		\
		"0x%6X 0x%6X 0x%6X 0x%6X\n",		\
		bytes[0],				\
		bytes[1],				\
		bytes[2],				\
		bytes[3],				\
		bytes[4],				\
		bytes[5],				\
		bytes[6],				\
		bytes[7]);				\
	fprintf(stderr,					\
		"d:%6d d:%6d d:%6d d:%6d "		\
		"d:%6d d:%6d d:%6d d:%6d\n",		\
		bytes[0],				\
		bytes[1],				\
		bytes[2],				\
		bytes[3],				\
		bytes[4],				\
		bytes[5],				\
		bytes[6],				\
		bytes[7]);				\
	binary_dumpc(bytes[0]);				\
	binary_dumpc(bytes[1]);				\
	binary_dumpc(bytes[2]);				\
	binary_dumpc(bytes[3]);				\
	binary_dumpc(bytes[4]);				\
	binary_dumpc(bytes[5]);				\
	binary_dumpc(bytes[6]);				\
	binary_dumpc(bytes[7]);				\
	printf("\n");


/*
 * convert an unsigned char[8] to 2 integers
 */
#define BLOCK_TO_INT(d, l, r)					\
	l = (d[0] << 24) | (d[1] << 16) | (d[2] << 8) | d[3];	\
	r = (d[4] << 24) | (d[5] << 16) | (d[6] << 8) | d[7];

/*
 * convert two integers to an unsigned char[8]
 */
#define INT_TO_BLOCK(d, l, r)					\
	d[0] = (l >> 24) & 0xff;				\
	d[1] = (l >> 16) & 0xff;				\
	d[2] = (l >> 8) & 0xff;					\
	d[3] = l & 0xff;					\
	d[4] = (r >> 24) & 0xff;				\
	d[5] = (r >> 16) & 0xff;				\
	d[6] = (r >> 8) & 0xff;					\
	d[7] = right & 0xff;

#define	BIT_POS(bit)	(7 - ((bit) % 8))
#define BYTE_POS(bit)	((bit) / 8)

#endif /* DES_H */
