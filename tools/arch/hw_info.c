#include <stdio.h>
#include <string.h>

static int isBigEndian(void)
{
	union {
		int number;
		char s;
	} test;
	memset(&test, 0, sizeof(test));
	test.number = 0x01000002;
	if (test.s == 0x01) {
		printf("big-endian\n");
		return 1;
	}
	printf("little-endian\n");
	return 0;
}

int main(int argc, char **argv)
{
	isBigEndian();
}
