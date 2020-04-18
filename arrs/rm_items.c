#include <stdio.h>

int main(void)
{
	/* int arr[10] = {1, 0, 4, 5, 0, 0, 5, 0, 2}; */
	/* int arr[10] = {0, 0, 0, 5, 0, 0, 5, 0, 2}; */
	/* int arr[10] = {0, 0, 0, 0, 0, 0, 0, 0, 2}; */
	int arr[10] = {0};

	int *p = &arr[0];
	int *n = &arr[1];
	int i, l;

	l = sizeof(arr) / sizeof(arr[0]);
	if (l <= 1)
		return 0;

	while (n != &arr[9]) {
		if (*p == 0) {
			while (*n == 0 && n != &arr[9])
				n++;
			if (n == &arr[9]) {
				if (*n != 0) {
					*p = *n;
					*n = 0;
				}
				break;
			}
			if (*n != 0) {
				*p = *n;
				*n = 0;
				p++;
				n++;
			}
		} else {
			p++;
			n++;
		}
	}

	for (i = 0; i < l; i++)
		printf("%d ", arr[i]);

	printf("\n");
}

