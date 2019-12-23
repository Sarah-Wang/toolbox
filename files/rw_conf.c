#include <stdio.h>
#include <errno.h>
#include <string.h>

#define MYFILE "./active_code"


//save secret as "secret=123456" in MYFILE
int read_param(char *file, char *rep, char *res)
{
	FILE *fp = NULL;
	int n = 0, len = 0;
	int find = 0;

	fp = fopen(file, "r");

	if (!fp) {
		printf("error: %s\n", strerror(errno));
		return -1;
	}

	while (fgets(res, 1024, fp)) {
		if (!strcmp(rep, "active_code="))
			n = sscanf(res, "active_code=%s", res);
		else if (!strcmp(rep, "secret="))
			n = sscanf(res, "secret=%s", res);
		else
			continue;
		if (n == 1) {
			len = strlen(res);
			if (res[len - 1] == '\n')
				res[len - 1] = '\0';
			printf("res is: %s\n", res);
			find = 1;
			break;
		}
	}
	fclose(fp);
	if (!find) {
		res[0] = '\0';
		return -1;
		}

	return 0;
}


//save secret as "123456" in MYFILE
int read_param_simple(char *file, char *res)
{
	FILE *fp = NULL;
	int n = 0, len = 0;

	fp = fopen(file, "r");

	if (!fp) {
		printf("error: %s\n", strerror(errno));
		return -1;
	}

	fgets(res, 1024, fp);
	len = strlen(res);
	if (res[len - 1] == '\n')
		res[len - 1] = '\0';
	printf("res is: %s\n", res);

	fclose(fp);
	return 0;
}

int save_param_simple(char *file, char *value)
{
	FILE *fp = NULL;
	int n = 0, len = 0;

	fp = fopen(file, "w");

	if (!fp) {
		printf("error: %s\n", strerror(errno));
		return -1;
	}
	n = fwrite(value, 1, strlen(value), fp);
	len = strlen(value);
	printf("write bytes: %d, value length: %d\n", n, len);

	fclose(fp);
	return 0;
}

void main(void)
{
	char active_code[256] = {0};
	char secret[256] = {0};
	int ret = 0;
	char value[128] = "123456";

	ret = read_param(MYFILE, "active_code=", active_code);
	if (!ret)
		printf("active code is: %s\n", active_code);

	ret = read_param_simple(MYFILE, active_code);
	if (!ret)
		printf("active code is: %s\n", active_code);

	ret = save_param_simple(MYFILE, value);

	if (!ret) {
		ret = read_param_simple(MYFILE, active_code);
		if (!ret)
			printf("active code is: %s\n", active_code);
	}
}
