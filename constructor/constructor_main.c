#include <stdio.h>
#include "constructor.h"

int main(void)
{
	int (*f)() = func;

	printf("I'm main in constructor.\n");
	return 0;
}
