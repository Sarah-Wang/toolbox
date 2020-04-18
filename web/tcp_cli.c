 /* Description : Simple Socket Client */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define FILE_LEN (1024 * 50)

int main(void)
{
	int cli_sock = 0;
	int inlen = 0;
	int outlen = 0;
	char out[20] = "hello from client\n";
	struct sockaddr_in svraddr = {0};
	char recvbuf[512] = {0};
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

	while (1) {
		outlen = send(cli_sock, out, sizeof(out), 0);
		if (outlen == -1) {
			perror("send");
			ret = -3;
			break;
		}
		inlen = recv(cli_sock, recvbuf, 511, 0);
		if (inlen == -1) {
			perror("recv");
			ret = -4;
			break;
		}
		recvbuf[inlen] = '\0';
		printf("recvbuf: %s\n", recvbuf);
		sleep(2);
	}
	close(cli_sock);
	return ret;
}
