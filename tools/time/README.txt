Q: How to compile?
A:
	1. mkdir -p build && cd build
	2. cmake ..
	3. make
	#4. make install

The target program is in build directory.


referrence: https://www.cnblogs.com/book-gary/p/3716790.html

struct tm *localtime(const time_t *clock);  //线程不安全
struct tm *localtime_r(const time_t *timer, struct tm *result);//线程安全
size_t strftime (char *ptr, size_t maxsize, const char *format, const struct tm *timeptr);
int gettimeofday(struct timeval *tv, struct timezone *tz);
