#include <stdio.h>

__attribute__((constructor)) int func(void)
{
	printf("I'm a func with constructor attribute.\n");
	return 0;
}
