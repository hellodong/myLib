#ifndef  _LOG_H_
#define  _LOG_H_


enum {
    LOG_MOD_DFT,
    LOG_MOD_1,
    LOG_MOD_MAX
};

#define LOG_MOD_FILE_PATH {\
    "log.log",\
    "1.log"\
}

enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_SYSTEM,
    LOG_LEVEL_MAX
};

#define __log_level_str {\
    "debug",\
    "info", \
    "warn", \
    "error",\
    "system"\
}

#define dftlog_debug(fmt,args...)    logger(LOG_MOD_DFT, LOG_LEVEL_DEBUG, __FUNCTION__,fmt, ##args)
#define dftlog_info(fmt,args...)     logger(LOG_MOD_DFT, LOG_LEVEL_INFO, __FUNCTION__,fmt, ##args)
#define dftlog_warn(fmt,args...)     logger(LOG_MOD_DFT, LOG_LEVEL_WARN, __FUNCTION__,fmt, ##args)
#define dftlog_error(fmt,args...)    logger(LOG_MOD_DFT, LOG_LEVEL_ERROR, __FUNCTION__,fmt, ##args)
#define dftlog_system(fmt,args...)   logger(LOG_MOD_DFT, LOG_LEVEL_SYSTEM, __FUNCTION__,fmt, ##args)


int loggernofmt(int _mod, int log_level, const char *_submod, const char *psmsg);

int logger(int _mod, int level, const char *_submod,const char *fmt, ...);


#endif
