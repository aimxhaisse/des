#include <stdio.h>
#include <string.h>

#include "des.h"

static __inline void des_ip1(union block *block)
{
	
}

void des_cipher_block(union block *block)
{
	des_ip1(block);
}
