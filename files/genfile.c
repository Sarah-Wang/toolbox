#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#define MIN 33
#define MAX 126
#define BASE_SIZE 1536

void usage(void)
{
	printf("Usage: ./%s <out file name> <num of bytes>\n", __FILE__);
	printf("This tool will produce a file with specified length ");
	printf("with random contents.\n");
}

void genrand(char *res, int n)
{
	int i;

	srand((unsigned int)time(NULL));
	for (i = 0; i < n; i++)
		res[i] = MIN + rand() % (MAX - MIN + 1);
}

int main(int argc, char **argv)
{
	int len = 0;
	char out[30] = {0};
	FILE *fd;
	char base[BASE_SIZE] = {0};
	int tmp, left;

	if (argc != 3) {
		usage();
		return -1;
	}

	len = atoi(argv[2]);
	if (len <= 0)
		return -1;

	snprintf(out, sizeof(out), "%s", argv[1]);
	fd = fopen(out, "wb");
	if (!fd) {
		printf("open %s failed\n", out);
		return -1;
	}

	left = len;
	while (left > 0) {
		tmp = left < BASE_SIZE ? left : BASE_SIZE;
		genrand(base, tmp);
		if (fwrite(base, 1, tmp, fd) < 0) {
			printf("write file error\n");
			break;
		}
		left = left - tmp;
	}
	if (fclose(fd) < 0)
		return -1;
	else
		return 0;
}
