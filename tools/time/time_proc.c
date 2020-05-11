#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//for time() and clock_gettime()
#include <time.h>

/* for usleep() */
#include <unistd.h>

/* for uint64_t */
#include <stdint.h>

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

static inline uint64_t get_time_us(void)
{
	struct timespec tv;

	clock_gettime(CLOCK_MONOTONIC, &tv);

	return (uint64_t)tv.tv_sec * 1000000 + tv.tv_nsec / 1000;
}

void test_get_time_us(void)
{
	printf("now time us: %llu\n", (unsigned long long int)get_time_us);
}

static int is_within_object_time(char *obj_time)
{
	time_t nsecs = 0;
	struct tm *ptm = NULL;
	int current_min = 0;
	int start_min = 0;
	int end_min = 0;
	int start_hour = 0;
	int end_hour = 0;
	int ret = 0;

	time(&nsecs);
	ptm = localtime(&nsecs);
	current_min = ptm->tm_hour * 60 + ptm->tm_min;
	ret = sscanf(obj_time, "%d:%d-%d:%d",
			&start_hour, &start_min,
			&end_hour, &end_min);
	if (ret != 4)
		return 0;
	start_min = start_hour * 60 + start_min;
	end_min = end_hour * 60 + end_min;

	if ((current_min < start_min) ||
			(current_min > end_min))
		return 0;
	else
		return 1;
}

int test_is_within_time(void)
{
	char obj_time[32] = "12:30-13:30";
	int res = is_within_object_time(obj_time);

	printf("now %s during %s\n", res ? "is" : "is not", obj_time);
}

void main(void)
{
	test_time_cost_timeval();
	test_time_cost_time();
	test_time_cost_timespec();
	get_time_string_ns();
	get_time_string_us();
	test_get_time_us();
	test_is_within_time();
}
