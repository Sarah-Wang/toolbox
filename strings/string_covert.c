#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> //for toupper()


#define IMG_PATH "/mnt/"

static int turn2ups(const char *src, char *dst)
{
	if (!src || !dst)
		return -1;
	while (*src)
		*dst++ = toupper(*src++);
	*dst = '\0';
	return 0;
}

void test_turn2ups(void)
{
	char src[20] = "axxxx42jijo0";
	char dst[20] = {0};

	turn2ups(src, dst);
	printf("%s\n\n", dst);
}

void str_to_mac(const char *buffer)
{
	char key[128] = {0};
	char value[128] = {0};
	int m = 0;
	int r = 0;
	char *tmps = NULL;
	char tmpx[32] = {0};
	long int tmp[6];
	char *fs = ":";
	/* char *buffer = "ethaddr=00:e0:4c:13:00:11"; */

	r = sscanf(buffer, "%[^=]=%s", key, value);
	if (r != 2) {
		printf("invalid format\n");
		return;
	}

	printf("value: %s\n", value);
	if (strncmp(key, "ethaddr", sizeof("ethaddr")) == 0) {
		tmps = strtok(value, fs);
		/* printf("tmps: %s\n", tmps); */
		sprintf(tmpx, "0x%s", tmps);
		/* printf("tmpx: %s\n", tmpx); */
		tmp[m] = strtol(tmpx, NULL, 16);
		/* tmp[m] = atoi(tmpx); */
		printf("tmp[m]=%02x\n", (unsigned int)tmp[m]);
		while (tmps = strtok(NULL, fs)) {
			/* printf("tmps: %s\n", tmps); */
			sprintf(tmpx, "0x%s", tmps);
			tmp[++m] = strtol(tmpx, NULL, 16);
			/* printf("tmpx: %s\n", tmpx); */
			printf("tmp[m]=%02x\n", (unsigned int)tmp[m]);
		}
	}
	printf("\n");
}

void str2mac(const char *buffer)
{
	char key[128] = {0};
	char value[128] = {0};
	int m = 0;
	char *tmps = NULL;
	int tmp[6];
	int i, r;
	unsigned char bytes[6];
	char *fs = ":";
	/* const char *buffer = "ethaddr=00:e0:4c:13:00:11"; */

	r = sscanf(buffer, "%[^=]=%s", key, value);
	if (r != 2) {
		printf("invalid format\n");
		return;
	}
	printf("value: %s\n", value);
	if (strncmp(key, "ethaddr", sizeof("ethaddr")) == 0) {
		r = sscanf(value, "%x:%x:%x:%x:%x:%x*c",
			&tmp[0], &tmp[1], &tmp[2],
			&tmp[3], &tmp[4], &tmp[5]);
		if (r == 6) {
			for (i = 0; i < 6; ++i) {
				bytes[i] = (unsigned char)tmp[i];
				printf("bytes[%d]=%02x\n", i, bytes[i]);
			}
		}
	}
	printf("\n");
}


void split_file_name(void)
{
	char oth[128] = {0};
	char *ptr = NULL;
	char name[128] = "/mnt/Sarah/0_1573695346.jpeg";
	char userid[128] = "Sarah";
	char pre[128]  = {0};
	int id, n;

	snprintf(pre, sizeof(pre), "%s%s/", IMG_PATH, userid);

	if ((strncmp(name, pre, strlen(pre))))
		return;
	printf("prefix match\n");
	ptr = name + strlen(pre);

	n = sscanf(ptr, "%d_%s", &id, oth);
	if (n == 2) {
		printf("oth: %s\n", oth);
		printf("id: %d\n\n", id);
	}
}


void main(void)
{
	char *buffer = "ethaddr=00:e0:4c:13:00:11";

	test_turn2ups();
	str_to_mac(buffer);
	str2mac(buffer);
	split_file_name();
}
