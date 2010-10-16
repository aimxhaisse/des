#include <stdio.h>

#include "des.h"

void binary_dumpc(unsigned char c)
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

void binary_dumpi(unsigned int c)
{
	int i;

	for(i = 31; i >= 0; i--) {
		if ((1 << i) & c)
			printf("1");
		else
			printf("0");
		if (i % 8 == 0)
			printf(" ");
	}
}
