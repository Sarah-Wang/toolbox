#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int file_seek(const char *name, unsigned long long offset)
{
	char buffer[20] = {0};
	int fd = 0;

	fd = open(name, O_RDONLY);
	if (fd < 0) {
		printf("open %s failed\n", name);
		return fd;
	}
	if (lseek(fd, offset, SEEK_SET) < 0) {
		printf("seek %s to %lld failed\n", name, offset);
		close(fd);
		return -1;
	}
	if (read(fd, buffer, 4) < 0) {
		printf("read err");
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}

int file_check(const char *file, const char *cont)
{
	char buf[30] = {0};
	int fd = 0, rd = 0;
	char act[20] = {0};

	fd = open(file, O_RDONLY);
	if (fd == -1) {
		printf("open %s failed!\n", file);
		return -1;
	}

	rd = read(fd, buf, 30);
	if (rd == -1) {
		printf("cannot read \"%s\"\n", file);
		goto out_error;
	}
	if (rd == 30) {
		printf("contents of \"%s\" is too long\n", file);
		goto out_error;
	}

	if (sscanf(buf, "%s\n", act) != 1) {
		printf("cannot read act from \"%s\"\n", file);
		goto out_error;
	}

	if (strcmp(cont, act)) {
		printf("cont is not %s\n", cont);
		goto out_error;
	}

	if (close(fd)) {
		printf("close failed on \"%s\"", file);
		return -1;
	}

	return 0;

out_error:
	close(fd);
	return -1;
}


void main(void)
{
	const char *file = "/home/sarah/action";
	const char *cont = "save";
	char name[20] = "test.txt";
	unsigned long long offset = 2;

	if (file_check(file, cont) < 0)
		printf("file not correct\n");

	if (file_seek(name, offset))
		printf("file seek failed\n");
}
