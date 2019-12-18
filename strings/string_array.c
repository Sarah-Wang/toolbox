#include <stdio.h>

#define  TRUE   1
#define  FALSE  0

void string_array_scan(void)
{
	const char *data[] = {"aa", "bb", "cc"};
	const char **p = data;

	while (*p) {
		printf("data: %s\n", *p);
		p++;
	}
}


/* check if a char exists in a string list */
int find_char(const char **strings, char value)
{
	const char *str;

	while ((str = *strings++) != NULL) {
		while (*str++ != '\0') {
			if (*str == value) {
				printf("here: %s\n", str);
				return TRUE;
			}
		}
	}
	return FALSE;
}

int main(void)
{
	const char *strings[] = {
		"Sarah Wang",
		"July Sun",
		"Bob Tang"
	};
	int ret = 0;

	ret = find_char(strings, 'o');
	printf("find result: %s\n", (ret == TRUE) ? "TRUE" : "FALSE");
	return 0;
}
