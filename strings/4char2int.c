#include <stdio.h>

void convert_chars_2_int(unsigned char a[], unsigned int *res)
{
	/* unsigned char a[4] = {0x00, 0x01, 0x02, 0x03}; */
	unsigned int b;

	b = (a[0] & 0x000000ff) |
	    ((a[1] << 8) & 0x0000ff00) |
	    ((a[2] << 16) & 0x00ff0000) |
	    ((a[3] << 24) & 0xff000000);
	/* printf("b = 0x%08x\n", b); */
	*res = b;
}

void main(void)
{
	unsigned char a[4] = {0x00, 0x01, 0x02, 0x03};
	unsigned int b;

	convert_chars_2_int(a, &b);
	printf("b = 0x%08x\n", b);
}
