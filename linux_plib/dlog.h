

#ifndef _DLOG_H_
#define _DLOG_H_

#define LOGLEV_ERR    (0)
#define LOGLEV_INF    (1)
#define LOGLEV_DBG    (2)

#define dlog_dbg(fmt, arg...)   do{dlog_printf(LOGLEV_DBG, "[%s] %s() #%d " fmt,"dbg", __FUNCTION__,__LINE__,##arg);}while(0)
#define dlog_inf(fmt, arg...)   do{dlog_printf(LOGLEV_INF, "[%s] %s() #%d " fmt,"inf", __FUNCTION__,__LINE__,##arg);}while(0)
#define dlog_err(fmt, arg...)   do{dlog_printf(LOGLEV_ERR, "[%s] %s() #%d " fmt,"err", __FUNCTION__,__LINE__,##arg);}while(0)


void dlog_setLevel(int level);

void dlog_identify_open(const char *identifyStr);

void dlog_identify_close(void);

void dlog_printf(int priority, const char *fmt, ...);

#endif
