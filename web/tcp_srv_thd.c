 /* Description : Simple Socket Server */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "common.h"

char st = ST_INIT;
int reset;
pthread_rwlock_t lck_st;
/* pthread_rwlock_t lck_re; */

static void *do_job(void)
{
	int i;

	pthread_detach(pthread_self());

	pthread_rwlock_wrlock(&lck_st);
	st = ST_JOB_DOING;
	pthread_rwlock_unlock(&lck_st);

	for (i = 0; i < 30 && !reset; i++)
		sleep(1);

	pthread_rwlock_wrlock(&lck_st);
	st = ST_JOB_DONE;

	if (reset) {
		reset = 0;
		st = ST_INIT;
	}
	pthread_rwlock_unlock(&lck_st);
}

static void *save_res(void)
{
	int i;

	pthread_detach(pthread_self());

	pthread_rwlock_wrlock(&lck_st);
	st = ST_SAVING_RES;
	pthread_rwlock_unlock(&lck_st);

	for (i = 0; i < 20 && !reset; i++)
		sleep(1);

	pthread_rwlock_wrlock(&lck_st);
	st = ST_SAVE_DONE;

	if (reset) {
		reset = 0;
		st = ST_INIT;
	}
	pthread_rwlock_unlock(&lck_st);
}

static int check_st(char *cmd)
{
	int ret = 0;

	pthread_rwlock_rdlock(&lck_st);
	if (!strcmp(cmd, CMD_GET_ST) || !strcmp(cmd, CMD_RESET)
			|| !strcmp(cmd, CMD_DISCONNECT))
		goto out;

	switch (st) {
	case ST_INIT:
		if (strcmp(cmd, CMD_DO_JOB))
			ret = -1;
		break;
	case ST_JOB_DONE:
		if (strcmp(cmd, CMD_SAVE_RES))
			ret = -1;
		break;
	case ST_JOB_DOING:
	case ST_SAVING_RES:
	case ST_SAVE_DONE:
	default:
		ret = -1;
		break;
	}
out:
	pthread_rwlock_unlock(&lck_st);
	return ret;
}

int main(int argc, char **argv)
{
	struct sockaddr_in saddr = {0};
	struct sockaddr_in caddr = {0};
	char buf[100] = {0};
	char str[100] = {0};
	int listen_sock = 0, conn_sock = 0;
	int addr_len = 0;
	int i, len, ret;
	pthread_t tid_job, tid_save;

	reset = 0;
	ret = pthread_rwlock_init(&lck_st, NULL);
	if (ret) {
		printf("lock st failed\n");
		return ret;
	}

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


	while (1) {
		conn_sock = accept(listen_sock,
				(struct sockaddr *)&caddr,
				&addr_len);
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

			if (buf == 0)
				break;

			buf[len] = '\0';
			printf("Recive from %s: %d\n",
				inet_ntop(AF_INET, &caddr.sin_addr,
					str, sizeof(str)),
				ntohs(caddr.sin_port));

			printf("buf: %s\n", buf);
			ret = check_st(buf);
			if (ret) {
				printf("check_st failed, cmd ignored.\n");
				break;
			}
			if (!strcmp(buf, CMD_GET_ST)) {
				pthread_rwlock_rdlock(&lck_st);
				len = send(conn_sock, &st, sizeof(st), 0);
				pthread_rwlock_unlock(&lck_st);
				if (len == -1) {
					perror("send");
					ret = -6;
					goto out;
				}
				printf("send status\n");
			}
			if (!strcmp(buf, CMD_DO_JOB)) {
				ret = pthread_create(&tid_job, NULL,
						(void *)do_job, NULL);
				if (ret) {
					printf("job failed\n");
						goto out;
				}
			}
			if (!strcmp(buf, CMD_SAVE_RES)) {
				ret = pthread_create(&tid_job, NULL,
						(void *)save_res, NULL);
				if (ret) {
					printf("save failed\n");
						goto out;
				}
			}
			if (!strcmp(buf, CMD_DISCONNECT)) {
				close(conn_sock);
				/* reset = 1; */
				pthread_rwlock_wrlock(&lck_st);
				st = ST_INIT;
				pthread_rwlock_unlock(&lck_st);
				break;
			}
			if (!strcmp(buf, CMD_RESET)) {
				reset = 1;
				pthread_rwlock_wrlock(&lck_st);
				st = ST_INIT;
				pthread_rwlock_unlock(&lck_st);
				sleep(2);
				reset = 0;
				break;
			}
			sleep(1);
		}
	}

out:
	close(listen_sock);

	return ret;
}
