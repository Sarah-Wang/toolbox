#include <stdio.h>
#include <string.h>

#define IMG_PATH "/mnt/"
void main(void)
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
		printf("id: %d\n", id);
	}
}

