//syslog saved in /var/log/syslog by default

#include <syslog.h>
#include <stdlib.h>
#include <stdio.h>

#define ADP_LOG(pro, fmt, args...) \
	syslog(pro, "%s[%d] "fmt, __func__, __LINE__, ##args)

#define ADP_ERR(...) ADP_LOG(LOG_ERR, __VA_ARGS__)
#define ADP_WARN(...) ADP_LOG(LOG_WARNING, __VA_ARGS__)
#define ADP_NOTE(...) ADP_LOG(LOG_NOTICE, __VA_ARGS__)
#define ADP_INFO(...) ADP_LOG(LOG_INFO, __VA_ARGS__)
#define ADP_DBG(...) ADP_LOG(LOG_DEBUG, __VA_ARGS__)


#define ADP_ERR_S(...) syslog(LOG_ERR, __VA_ARGS__)
#define ADP_WARN_S(...) syslog(LOG_WARNING, __VA_ARGS__)
#define ADP_NOTE_S(...) syslog(LOG_NOTICE, __VA_ARGS__)
#define ADP_INFO_S(...) syslog(LOG_INFO, __VA_ARGS__)
#define ADP_DEBUG_S(...) syslog(LOG_DEBUG, __VA_ARGS__)

int get_log_mask(int level)
{
	int mask = 0;

	switch (level) {
	case 0:
		mask = LOG_MASK(LOG_CRIT);
		break;
	case 1:
		mask = LOG_MASK(LOG_CRIT) | LOG_MASK(LOG_ERR);
	case 2:
		mask = LOG_MASK(LOG_CRIT) | LOG_MASK(LOG_ERR)
			| LOG_MASK(LOG_WARNING);
		break;
	case 3:
		mask = LOG_MASK(LOG_CRIT) | LOG_MASK(LOG_ERR)
			| LOG_MASK(LOG_WARNING) | LOG_MASK(LOG_NOTICE);
		break;
	case 4:
		mask = LOG_MASK(LOG_CRIT) | LOG_MASK(LOG_ERR)
			| LOG_MASK(LOG_WARNING) | LOG_MASK(LOG_NOTICE)
			| LOG_MASK(LOG_INFO);
		break;
	case 5:
		mask = LOG_MASK(LOG_CRIT) | LOG_MASK(LOG_ERR)
			| LOG_MASK(LOG_WARNING) | LOG_MASK(LOG_NOTICE)
			| LOG_MASK(LOG_INFO) | LOG_MASK(LOG_DEBUG);
		break;
	default:
		mask = LOG_MASK(LOG_CRIT) | LOG_MASK(LOG_ERR)
			| LOG_MASK(LOG_WARNING);
		break;
	}
	return mask;
}

void simple_log(void)
{
	int mask =
		LOG_MASK(LOG_EMERG)
		| LOG_MASK(LOG_ALERT)
		| LOG_MASK(LOG_CRIT)
		| LOG_MASK(LOG_ERR)
		| LOG_MASK(LOG_WARNING)
		/* | LOG_MASK(LOG_NOTICE) */
		/* | LOG_MASK(LOG_INFO) */
		| LOG_MASK(LOG_DEBUG)
		;
	/* openlog("ADP", LOG_CONS | LOG_PID, LOG_LOCAL0); */
	/* openlog("ADP", LOG_CONS | LOG_PID, LOG_LOCAL7); */
	openlog("ADP", LOG_CONS | LOG_PID, 0);

	setlogmask(mask);
	ADP_ERR("this is a err\n");
	ADP_INFO("this is a info\n");
	ADP_DBG("this is a debug\n");
}


int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("usage: %s <log_leve>([0-5])\n", argv[0]);
		return -1;
	}

	int level = 0;
	int mask = 0;

	level = atoi(argv[1]);
	printf("level = %d\n", level);
	mask = get_log_mask(level);
	openlog("ADP", 0, 0);
	setlogmask(mask);
	ADP_ERR("this is a err\n");
	ADP_INFO("this is a info\n");
	ADP_DBG("this is a debug\n");
	/* closelog(); */
	return 0;
}

