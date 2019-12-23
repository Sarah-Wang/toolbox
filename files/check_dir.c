#include <stdio.h>
#include <unistd.h>
#include <string.h>

int check_path(char *filename)
{
	char path[256] = {0};
	char *ptr = NULL;
	int len = 0;
	int ret = 0;

	if (!filename) {
		printf("ERROR: filename is NULL!\n");
		return -1;
	}

	ptr = strrchr(filename, '/');
	if (!ptr) {
		printf("ERROR: filename must be a path!\n");
		return -2;
	}

	len = ptr - filename;
	if (len >= sizeof(path) ) {
		printf("ERROR: filename is too long( >= %d )!\n",
				(int)sizeof(path));
		return -3;
	}

	memcpy(path, filename, len);
	path[len] = '\0';

	ret = access(path, R_OK | W_OK);
	if (ret) {
		printf("ERROR: cannot access path %s\n", path);
		return -4;
	}
	return ret;
}

int check_file(char *filename)
{
	char path[256] = {0};
	int ret = 0;

	ret = access(filename, F_OK | R_OK);
	if (ret) {
		printf("ERROR: cannot access file %s\n", filename);
		return -5;
	}
	return ret;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("usage: check <filename>\n\n");
		return 1;
	}
	if (check_path(argv[1]))
		printf("check directory failed!\n");
	if (check_file(argv[1]))
		printf("check file failed!\n");

	return 0;
}
