//refer to libwebsockets
#include <syslog.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <unistd.h>

#define ARRAY_SIZE(_x) (sizeof(_x) / sizeof(_x[0]))

static char tty;

enum lws_log_levels {
	LLL_ERR		= 1 << 0,
	LLL_WARN	= 1 << 1,
	LLL_NOTICE	= 1 << 2,
	LLL_INFO	= 1 << 3,
	LLL_DEBUG	= 1 << 4,
	LLL_PARSER	= 1 << 5,
	LLL_HEADER	= 1 << 6,
	LLL_EXT		= 1 << 7,
	LLL_CLIENT	= 1 << 8,
	LLL_LATENCY	= 1 << 9,
	LLL_USER	= 1 << 10,
	LLL_THREAD	= 1 << 11,

	LLL_COUNT	= 12 /* set to count of valid flags */
};

int log_level = LLL_ERR | LLL_WARN | LLL_NOTICE;

static const char * const colours[] = {
	"[31;1m", /* LLL_ERR */
	"[36;1m", /* LLL_WARN */
	"[35;1m", /* LLL_NOTICE */
	"[32;1m", /* LLL_INFO */
	"[34;1m", /* LLL_DEBUG */
	"[33;1m", /* LLL_PARSER */
	"[33m", /* LLL_HEADER */
	"[33m", /* LLL_EXT */
	"[33m", /* LLL_CLIENT */
	"[33;1m", /* LLL_LATENCY */
	"[30;1m", /* LLL_USER */
	"[31m", /* LLL_THREAD */
};

static const char * const log_level_names[] = {
	"E",
	"W",
	"N",
	"I",
	"D",
	"P",
	"H",
	"EXT",
	"C",
	"L",
	"U",
	"T",
	"?",
	"?"
};


static void lwsl_emit_stderr(int level, const char *line);
static void (*lwsl_emit)(int level, const char *line) = lwsl_emit_stderr;


int64_t lws_now_usecs(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((unsigned long long)tv.tv_sec * 1000000LL) + tv.tv_usec;
}


int lwsl_timestamp(int level, char *p, int len)
{
	unsigned long long now;
	struct tm tm;
	time_t o_now = time(NULL);
	struct tm *ptm = NULL;
	int n;

	if (localtime_r(&o_now, &tm))
		ptm = &tm;
	p[0] = '\0';
	for (n = 0; n < LLL_COUNT; n++) {
		if (level != (1 << n))
			continue;
		now = lws_now_usecs() / 100;
		if (ptm)
			n = snprintf(p, len,
				"[%04d/%02d/%02d %02d:%02d:%02d:%04d] %s: ",
				ptm->tm_year + 1900,
				ptm->tm_mon + 1,
				ptm->tm_mday,
				ptm->tm_hour,
				ptm->tm_min,
				ptm->tm_sec,
				(int)(now % 10000), log_level_names[n]);
		else
			n = snprintf(p, len, "[%llu:%04d] %s: ",
					(unsigned long long) now / 10000,
					(int)(now % 10000), log_level_names[n]);
		return n;
	}
	return 0;
}


void lwsl_emit_stderr(int level, const char *line)
{
	char buf[50];
	int n, m = ARRAY_SIZE(colours) - 1;

	if (!tty)
		tty = isatty(2) | 2;
	lwsl_timestamp(level, buf, sizeof(buf));

	if (tty == 3) {
		n = 1 << (ARRAY_SIZE(colours) - 1);
		while (n) {
			if (level & n)
				break;
			m--;
			n >>= 1;
		}
		fprintf(stderr, "%c%s%s%s%c[0m", 27, colours[m], buf, line, 27);
	} else
		fprintf(stderr, "%s%s", buf, line);
}



void _lws_logv(int filter, const char *format, va_list vl)
{
	static char buf[256];
	int n;

	if (!(log_level & filter))
		return;

	n = vsnprintf(buf, sizeof(buf) - 1, format, vl);
	(void)n;
	/* vnsprintf returns what it would have written, even if truncated */
	if (n > (int)sizeof(buf) - 1) {
		n = sizeof(buf) - 5;
		buf[n++] = '.';
		buf[n++] = '.';
		buf[n++] = '.';
		buf[n++] = '\n';
		buf[n] = '\0';
	}
	if (n > 0)
		buf[n] = '\0';
	lwsl_emit(filter, buf);
}

void _lws_log(int filter, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	_lws_logv(filter, format, ap);
	va_end(ap);
}

void lws_set_log_level(int level, void (*func)(int level, const char *line))
{
	log_level = level;
	if (func)
		lwsl_emit = func;
}

#define lwsl_err(...) _lws_log(LLL_ERR, __VA_ARGS__)
#define lwsl_user(...) _lws_log(LLL_USER, __VA_ARGS__)
#define lwsl_warn(...) _lws_log(LLL_WARN, __VA_ARGS__)
#define lwsl_notice(...) _lws_log(LLL_NOTICE, __VA_ARGS__)
#define lwsl_info(...) _lws_log(LLL_INFO, __VA_ARGS__)
#define lwsl_debug(...) _lws_log(LLL_DEBUG, __VA_ARGS__)
#define lwsl_parser(...) _lws_log(LLL_PARSER, __VA_ARGS__)
#define lwsl_header(...)  _lws_log(LLL_HEADER, __VA_ARGS__)
#define lwsl_ext(...)  _lws_log(LLL_EXT, __VA_ARGS__)
#define lwsl_client(...) _lws_log(LLL_CLIENT, __VA_ARGS__)
#define lwsl_latency(...) _lws_log(LLL_LATENCY, __VA_ARGS__)
#define lwsl_thread(...) _lws_log(LLL_THREAD, __VA_ARGS__)

int main(void)
{
	int logs =
		LLL_USER
		| LLL_ERR
		| LLL_WARN
		| LLL_NOTICE
		/* | LLL_INFO */
		/* | LLL_PARSER */
		/* | LLL_HEADER */
		/* | LLL_EXT */
		/* | LLL_CLIENT */
		/* | LLL_LATENCY */
		/* | LLL_DEBUG */
		;

	lws_set_log_level(logs, NULL);
	lwsl_err("this is a err\n");
	lwsl_user("this is a user\n");
	lwsl_notice("this is a notice\n");
	lwsl_info("this is a info\n");
	lwsl_warn("this is a warn\n");
	lwsl_debug("this is a debug\n");
}
