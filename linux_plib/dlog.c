

#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>

#include "dlog.h"

#define USE_SYSLOG	(0)


static int gLog_level=LOGLEV_INF;

static const int log_class[] = {
    [LOGLEV_ERR] = LOG_CRIT,
    [LOGLEV_INF] = LOG_INFO,
    [LOGLEV_DBG] = LOG_DEBUG
};



void dlog_setLevel(int level)
{
  gLog_level=level;
}

void dlog_identify_open(const char *identifyStr)
{
	if(USE_SYSLOG){
		openlog(identifyStr, 0, LOG_DAEMON);
	}
}

void dlog_identify_close(void)
{
	if(USE_SYSLOG){
		closelog();
	}
}

void dlog_printf(int priority, const char *fmt, ...)
{
    va_list vl; 

    if (priority > gLog_level) {
        return;
    }

    va_start(vl, fmt);
    if (USE_SYSLOG){
        vsyslog(log_class[priority], fmt, vl);
    } else{
        vfprintf(stderr, fmt, vl);
    }
    va_end(vl);
}

