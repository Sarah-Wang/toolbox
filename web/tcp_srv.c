 /* Description : Simple Socket Server */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
	struct sockaddr_in saddr = {0};
	struct sockaddr_in caddr = {0};
	char buf[100] = {0};
	char str[100] = {0};
	int listen_sock = 0, conn_sock = 0;
	int addr_len = 0;
	int i, len, ret;

	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == -1) {
		perror("socket");
		return -1;
	}

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(6688);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY); //any address
	ret = bind(listen_sock, (struct sockaddr *)&saddr, 16);
	if (ret == -1) {
		perror("bind");
		return -2;
	}

	ret = listen(listen_sock, 20);
	if (ret == -1) {
		perror("listen");
		return -3;
	}

	printf("Accepting connections ...\n");
	conn_sock = accept(listen_sock, (struct sockaddr *)&caddr, &addr_len);
	if (conn_sock == -1) {
		perror("accept");
		ret = -4;
		goto out;
	}

	while (1) {
		len = recv(conn_sock, buf, 100, 0);
		if (len == -1) {
			perror("recv");
			ret = -5;
			break;
		}

		printf("Recive from %s: %d\n",
			inet_ntop(AF_INET, &caddr.sin_addr, str, sizeof(str)),
			ntohs(caddr.sin_port));

		for (i = 0; i < len; i++)
			buf[i] = toupper(buf[i]);

		len = send(conn_sock, buf, len + 1, 0);
		if (len == -1) {
			perror("send");
			ret = -6;
			break;
		}

		printf("Send : %s\n", buf);
	}

	close(conn_sock);
out:
	close(listen_sock);

	return ret;
}
