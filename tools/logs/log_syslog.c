//syslog saved in /var/log/syslog by default

#include <syslog.h>

#define ADP_LOG(pro, fmt, args...) \
	syslog(pro, "%s[%d] "fmt, __func__, __LINE__, ##args)

#define ADP_ERR(...) ADP_LOG(LOG_ERR, __VA_ARGS__)
#define ADP_WARN(...) ADP_LOG(LOG_WARNING, __VA_ARGS__)
#define ADP_NOTE(...) ADP_LOG(LOG_NOTICE, __VA_ARGS__)
#define ADP_INFO(...) ADP_LOG(LOG_INFO, __VA_ARGS__)
#define ADP_DBG(...) ADP_LOG(LOG_DEBUG, __VA_ARGS__)


#define ADP_ERR_S(...) syslog(LOG_ERR, __VA_ARGS__)
#define ADP_INFO_S(...) syslog(LOG_INFO, __VA_ARGS__)
#define ADP_DEBUG_S(...) syslog(LOG_DEBUG, __VA_ARGS__)
#define ADP_DEBUG_S(...) syslog(LOG_DEBUG, __VA_ARGS__)


int main(void)
{
	int mask =
		LOG_MASK(LOG_EMERG)
		| LOG_MASK(LOG_ALERT)
		| LOG_MASK(LOG_CRIT)
		| LOG_MASK(LOG_ERR)
		/* | LOG_MASK(LOG_WARNING) */
		/* | LOG_MASK(LOG_NOTICE) */
		/* | LOG_MASK(LOG_INFO) */
		/* | LOG_MASK(LOG_DEBUG) */
		;
	/* openlog("ADP", LOG_CONS | LOG_PID, LOG_LOCAL0); */
	/* openlog("ADP", LOG_CONS | LOG_PID, LOG_LOCAL7); */
	/* openlog("ADP", LOG_CONS | LOG_PID, 0); */
	openlog("ADP", 0, 0);
	setlogmask(mask);
	ADP_ERR("this is a err\n");
	ADP_INFO("this is a info\n");
	ADP_DBG("this is a debug\n");
	/* closelog(); */
	return 0;
}

