#include <sys/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void _adp_log(int filter, const char *format, ...);

#define adpl_err(...) _adp_log(LL_ERR, __VA_ARGS__)
#define adpl_warn(...) _adp_log(LL_WARN, __VA_ARGS__)
#define adpl_notice(...) _adp_log(LL_NOTICE, __VA_ARGS__)
#define adpl_info(...) _adp_log(LL_INFO, __VA_ARGS__)
#define adpl_debug(...) _adp_log(LL_DEBUG, __VA_ARGS__)
#define adpl_user(...) _adp_log(LL_USER, __VA_ARGS__)

enum adp_log_levels {
	LL_ERR		= 1 << 0,
	LL_WARN		= 1 << 1,
	LL_NOTICE	= 1 << 2,
	LL_INFO		= 1 << 3,
	LL_DEBUG	= 1 << 4,
	LL_USER		= 1 << 5,
	LL_COUNT	= 6 /* set to count of valid flags */
};

int log_level = LL_ERR | LL_WARN | LL_NOTICE;

void _adp_logv(int filter, const char *format, va_list vl)
{
	static char buf[256];
	int n;

	if (!(log_level & filter))
		return;

	n = vsnprintf(buf, sizeof(buf) - 1, format, vl);
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
	fprintf(stderr, "%s", buf);
}

void _adp_log(int filter, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	_adp_logv(filter, format, ap);
	va_end(ap);
}

int main(void)
{
	int log_level =
		/* LL_ERR */
		LL_USER
		| LL_WARN
		/* | LL_NOTICE */
		/* | LL_USER */
		;

	adpl_err("this is a err\n");
	adpl_user("this is a user\n");
	adpl_notice("this is a notice\n");
	adpl_info("this is a info\n");
	adpl_warn("this is a warn\n");
	adpl_debug("this is a debug\n");
}
