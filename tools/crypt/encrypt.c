#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <openssl/sha.h>
#include <openssl/md5.h>

static int get_sha1_string(char *src, char *dst)
{
	unsigned char digest[SHA_DIGEST_LENGTH];
	int i, ret = 1;
	SHA_CTX ctx;

	ret = SHA1_Init(&ctx);
	if (!ret)
		return -1;
	ret = SHA1_Update(&ctx, src, strlen(src));
	if (!ret)
		return -1;
	ret = SHA1_Final(digest, &ctx);
	if (!ret)
		return -1;

	for (i = 0; i < SHA_DIGEST_LENGTH; i++)
		sprintf(&dst[i*2], "%02x", (unsigned int)digest[i]);
	return 0;
}



static int get_md5sum_string(char *filename, char *dst)
{
	MD5_CTX ctx;
	unsigned char outmd[33] = {0};
	char buffer[1024] = {0};
	int len = 0;
	int i = 0;
	FILE *fp = NULL;

	fp = fopen(filename, "rb");
	if (!fp) {
		printf("Can't open file\n");
		return -1;
	}

	MD5_Init(&ctx);
	len = fread(buffer, 1, 1024, fp);
	while (len > 0) {
		MD5_Update(&ctx, buffer, len);
		memset(buffer, 0, sizeof(buffer));
		len = fread(buffer, 1, 1024, fp);
	}
	MD5_Final(outmd, &ctx);

	for (i = 0; i < 16; i++)
		sprintf(&dst[i*2], "%02x", (unsigned int)outmd[i]);

	fclose(fp);

	return 0;
}



void main(void)
{
	char dst[33] = {0};
	char filename[64] = "./test.txt";
	int ret = 0;

	//0ce3b5c19f6bab15e892c11260275bce
	ret = get_md5sum_string(filename, dst);
	if (!ret)
		printf("%s md5sum: %s\n", filename, dst);
}
