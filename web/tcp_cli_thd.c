 /* Description : Simple Socket Client */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "common.h"

static int get_st(int cli_sock, int st)
{
	int outlen = 0;
	int inlen = 0;
	char in[20] = {0};
	int ret = 0;

	while (1) {
		/* printf("%s %d\n", __func__, __LINE__); */
		outlen = send(cli_sock, CMD_GET_ST, sizeof(CMD_GET_ST), 0);
		if (outlen == -1) {
			perror("send");
			ret = -3;
			goto out;
		}
		sleep(1); //necessary to wait for server
		inlen = recv(cli_sock, in, 1, 0);
		if (inlen == -1) {
			perror("recv");
			ret = -4;
			goto out;
		}
		printf("recv st: %d\n", in[0]);
		if (in[0] == st)
			break;
		sleep(3);
	}
out:
	return ret;
}

int main(void)
{
	int cli_sock = 0;
	int inlen = 0;
	int outlen = 0;
	struct sockaddr_in svraddr = {0};
	char in[20] = {0};
	int ret = 0;

	cli_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (cli_sock < 0) {
		perror("socket");
		return -1;
	}

	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(6688);
	svraddr.sin_addr.s_addr = inet_addr("10.0.2.108");

	ret = connect(cli_sock, (struct sockaddr *)&svraddr, sizeof(svraddr));
	if (ret < 0) {
		perror("connect");
		return -2;
	}

	ret = get_st(cli_sock, ST_INIT);
	if (ret) {
		printf("get st failed, ret = %d\n", ret);
		goto out;
	}

	outlen = send(cli_sock, CMD_DO_JOB, sizeof(CMD_DO_JOB), 0);
	if (outlen == -1) {
		perror("send");
		ret = -3;
	}

	ret = get_st(cli_sock, ST_JOB_DONE);
	if (ret) {
		printf("get st failed, ret = %d\n", ret);
		goto out;
	}

	outlen = send(cli_sock, CMD_SAVE_RES, sizeof(CMD_SAVE_RES), 0);
	if (outlen == -1) {
		perror("send");
		ret = -3;
	}

	ret = get_st(cli_sock, ST_SAVE_DONE);
	if (ret) {
		printf("get st failed, ret = %d\n", ret);
		goto out;
	}

	outlen = send(cli_sock, CMD_DISCONNECT, sizeof(CMD_DISCONNECT), 0);
	if (outlen == -1) {
		perror("send");
		ret = -3;
	}

out:
	close(cli_sock);
	return ret;
}
