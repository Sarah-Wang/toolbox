#include <stdio.h>
#include <stdlib.h>

void array_size_test(void)
{
	int a[][2] = { {1, 2}, {3, 4} };

	printf("sizeof(a) = %d\n", (int)sizeof(a));
	printf("sizeof(a[0]) = %d\n", (int)sizeof(a[0]));
}

void int_overflow_test(void)
{
	unsigned char x1 = 0xff;
	signed char x2 = 0x7f;
	signed char x = 0x7f;
	signed char y = 0x05;
	signed char r = x * y;

	printf("++x1 = %d\n", ++x1);
	printf("++x2 = %d\n", ++x2);
	printf("x*y = %d\n", r);
}

void sizeof_const_string_test(void)
{
	printf("sizeof(\"string\") is %d\n.",
			(unsigned int)sizeof("string"));
}

void ptr_test(void)
{
	char *value = "value";
	char *ffm = NULL;

	/* const char *ffm = NULL; */
	/* char const **v = &ffm; */
	/* *v = value; */
	/* printf("*v: %s\n", *v); */

	ffm = value;
	printf("ffm: %s\n", ffm);

	free(ffm);
	printf("after free ffm\n");
	printf("value: %s\n", value);
}

int main(void)
{
	array_size_test();

	int_overflow_test();

	sizeof_const_string_test();

	ptr_test();

	return 0;
}
