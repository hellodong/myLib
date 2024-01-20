#ifndef  _LOG_H_
#define  _LOG_H_


enum {
    LOG_MOD_DFT,
    LOG_MOD_1,
    LOG_MOD_MAX
};

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

#define LOG_MOD_FILE_PATH {"log.log", "1.log"}


int loggernofmt(int _mod, int log_level, const char *_submod, const char *psmsg);

#endif
