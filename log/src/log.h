#ifndef  _LOG_H_
#define  _LOG_H_

#ifdef __cplusplus
extern "C"{
#endif

enum {
    LOG_MOD_DFT,
    LOG_MOD_1,
    LOG_MOD_2,
    LOG_MOD_3,
    LOG_MOD_4,
    LOG_MOD_5,
    LOG_MOD_6,
    LOG_MOD_7,
    LOG_MOD_MAX
};

#define LOG_MOD_FILE_PATH {\
    "/tmp/dlog/log.log",\
    "/tmp/dlog/log_1.log",\
    "/tmp/dlog/log_2.log",\
    "/tmp/dlog/log_3.log",\
    "/tmp/dlog/log_4.log",\
    "log_5.log",\
    "log_6.log",\
    "log_7.log"\
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

#define log_debug(fmt,args...)    logger(LOG_MOD_DFT, LOG_LEVEL_DEBUG, __FUNCTION__,fmt, ##args)
#define log_info(fmt,args...)     logger(LOG_MOD_DFT, LOG_LEVEL_INFO, __FUNCTION__,fmt, ##args)
#define log_warn(fmt,args...)     logger(LOG_MOD_DFT, LOG_LEVEL_WARN, __FUNCTION__,fmt, ##args)
#define log_error(fmt,args...)    logger(LOG_MOD_DFT, LOG_LEVEL_ERROR, __FUNCTION__,fmt, ##args)
#define log_system(fmt,args...)   logger(LOG_MOD_DFT, LOG_LEVEL_SYSTEM, __FUNCTION__,fmt, ##args)


int loggernofmt(int _mod, int log_level, const char *_submod, const char *psmsg);

int logger(int _mod, int level, const char *_submod,const char *fmt, ...);

int log_edit_level(int _mod, unsigned int level);

unsigned long log_edit_file_size(int _mod, unsigned long file_size);


#ifdef __cplusplus
}
#endif

#endif
