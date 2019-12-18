#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//for time() and clock_gettime()
#include <time.h>

/* for usleep() */
#include <unistd.h>

int get_time_cost_timeval(struct timeval *result,
		struct timeval *old, struct timeval *new)
{
	if (old->tv_sec > new->tv_sec)
		return -1;
	if ((old->tv_sec == new->tv_sec) && (old->tv_usec > new->tv_usec))
		return -1;
	result->tv_sec = new->tv_sec - old->tv_sec;
	result->tv_usec = new->tv_usec - old->tv_usec;

	if (result->tv_usec < 0) {
		result->tv_sec--;
		result->tv_usec += 1000000;
	}

	printf("time cost: %lds and %ldus\n", result->tv_sec, result->tv_usec);
	return 0;
}

int test_time_cost_timeval(void)
{
	struct timeval start, stop, diff;

	gettimeofday(&start, NULL);

	printf("do something...\n");
	usleep(1000 * 3000);

	gettimeofday(&stop, NULL);
	get_time_cost_timeval(&diff, &start, &stop);
	printf("time cost: %lds and %ldus\n", diff.tv_sec, diff.tv_usec);
	return 0;
}


int get_time_cost_time(time_t old, time_t new)
{
	printf("time cost: %ld\n", new - old);
	return 0;
}

int test_time_cost_time(void)
{
	time_t start, stop;

	time(&start);

	printf("do something...\n");
	usleep(1000 * 3000);

	time(&stop);
	get_time_cost_time(start, stop);
}

int get_time_cost_timespec(struct timespec old, struct timespec new)
{
	struct timespec result = {0};

	if (old.tv_sec > new.tv_sec)
		return -1;
	if ((old.tv_sec == new.tv_sec) && (old.tv_nsec > new.tv_nsec))
		return -1;
	result.tv_sec = new.tv_sec - old.tv_sec;
	result.tv_nsec = new.tv_nsec - old.tv_nsec;

	if (result.tv_nsec < 0) {
		result.tv_sec--;
		result.tv_nsec += 1000000000;
	}

	printf("time cost: %lds and %ldus\n", result.tv_sec, result.tv_nsec);
	return 0;
}

void test_time_cost_timespec(void)
{
	struct timespec start, stop;

	clock_gettime(CLOCK_REALTIME, &start);

	printf("do something...\n");
	usleep(1000 * 3000);

	clock_gettime(CLOCK_REALTIME, &stop);
	get_time_cost_timespec(start, stop);
}

void get_time_string_ns(void)
{
	struct timespec ts;
	struct tm t;
	char date_time[64] = {0};

	clock_gettime(CLOCK_REALTIME, &ts);
	printf("clock_gettime: tv_sec=%ld, tv_nsec=%ld\n",
			ts.tv_sec, ts.tv_nsec);
	strftime(date_time, sizeof(date_time),
		"%Y-%m-%d %H:%M:%S", localtime_r(&ts.tv_sec, &t));
	printf("clock_gettime : date_time=%s, tv_nsec=%ld\n",
			date_time, ts.tv_nsec);
}

void get_time_string_us(void)
{
	struct timeval ts;
	struct tm t;
	char date_time[64] = {0};

	gettimeofday(&ts, NULL);
	printf("gettimeofday: tv_sec=%ld, tv_usec=%ld\n",
			ts.tv_sec, ts.tv_usec);
	strftime(date_time, sizeof(date_time),
		"%Y-%m-%d %H:%M:%S", localtime_r(&ts.tv_sec, &t));
	printf("gettimeofday: date_time=%s, tv_nsec=%ld\n",
			date_time, ts.tv_usec);
}


void main(void)
{
	test_time_cost_timeval();
	test_time_cost_time();
	test_time_cost_timespec();
	get_time_string_ns();
	get_time_string_us();
}
