#include <stdio.h>
#include <string.h>

struct wsclient {
	unsigned char optcode;
	unsigned char masklen;
	unsigned char maskkey[4];
	unsigned char data[5];
/* } __attribute__((packed)); */
} __packed;

void main(void)
{
	struct wsclient client;
	FILE *fp = NULL;
	int size = 0;
	unsigned char dLength = 11;
	unsigned char dAddress = 0x09;

	memset(&client, 0, sizeof(client));
	client.optcode = 0x8A;
	client.masklen = 0x84;
	client.maskkey[0] = 0x01;
	client.maskkey[1] = 0x02;
	client.maskkey[2] = 0x03;
	client.maskkey[3] = 0x04;
	strcpy(client.data, "pong");

	fp = fopen("./client.txt", "wb");
	if (!fp)
		printf("open file failed!\n");
	else {
		size = fwrite(&client, sizeof(client), 1, fp);
		size += fwrite(&dAddress, sizeof(dAddress), 1, fp);
		size += fwrite(&client, sizeof(client), 1, fp);
		printf("write size = %d\n", size);
		fclose(fp);
	}
}
