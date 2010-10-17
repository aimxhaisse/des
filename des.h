
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
	EBC /* electronic codebook, feel free to implement yours :) */
};

struct des {
	enum operation	op;	/* current operation */
	enum ciphermode	mode;	/* current mode */
	unsigned char	key[8];	/* key to use */
	unsigned char	subkeys[16][6];
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
void des_generate_subkeys(unsigned char *key, unsigned char (*subkeys)[6]);

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

#define	BIT_POS(bit)	(7 - ((bit) % 8))
#define BYTE_POS(bit)	((bit) / 8)

#endif /* DES_H */
