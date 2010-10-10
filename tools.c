#include <stdio.h>

#include "des.h"

void binary_dump(unsigned char c)
{
	int i;

	for(i = 7; i >= 0; i--) {
		if ((1 << i) & c)
			printf("1");
		else
			printf("0");
	}
	printf(" ");
}
