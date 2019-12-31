#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#define BUF_LEN 100

struct info {
	long int num;
	char *str;
};

void *speak(void *data)
{
	/* pthread_detach(pthread_self()); */

	while (1) {
		printf("Speaking from the detached thread...\n");
		sleep(1);
	}
	pthread_exit(NULL);
}

int test_detached_thread(void)
{
	pthread_t handle;

	if (!pthread_create(&handle, NULL, speak, NULL)) {
		printf("Thread create successfully !!!\n");
		if (!pthread_detach(handle))
			printf("Thread detached successfully !!!\n");
	}

	sleep(3);
	printf("%s thread terminate...\n", __func__);
	return 0;
}

void *add_num(void *ptr)
{
	static int g;
	struct info *inf = (struct info *)ptr;

	sleep(1);
	inf->num = (long int)pthread_self();

	inf->str = realloc(inf->str, BUF_LEN);
	snprintf(inf->str, BUF_LEN, "this is set in thread %ld", inf->num);
	printf("%s inf->str: %s\n", __func__, inf->str);

	sleep(1);
	pthread_exit(0);
}

void test_joinable_thread(void)
{
	pthread_t thread1;
	struct info inf = {0};
	int i;
	int ret = 0;

	for (i = 0; i < 2; i++) {
		ret = pthread_create(&thread1, NULL,
				(void *)add_num, (void *)&inf);
		if (ret) {
			printf("pthread_create failed, ret = %d\n", ret);
			continue;
		}
		pthread_join(thread1, NULL);
		printf("%s str is %s\n", __func__, inf.str);
		printf("%s num is %ld\n", __func__, inf.num);
	}
	printf("%s thread terminate...\n", __func__);
}

void main(void)
{
	/* test_joinable_thread(); */
	test_detached_thread();
}
