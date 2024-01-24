

#include "log.h"
#include "log_buf.h"
#include "log_file.h"

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>


#define LOG_FILE_STR_MAX    (256)

typedef struct _logMod
{
    char log_file_path[LOG_FILE_STR_MAX];
    int log_level;
    pthread_mutex_t mutx;
    stLogBuff_t buff;
}stLogMod_t;

pthread_t log2file_thread;
stLogMod_t g_log_mod[LOG_MOD_MAX];

const char *g_level2str[LOG_LEVEL_MAX] = __log_level_str;


static void *_log2file(void *arg);

static int __log_module_init(stLogMod_t *log_mod, int _mod, int log_level)
{
    const char* __file_name[LOG_MOD_MAX] = LOG_MOD_FILE_PATH;
    if (NULL == log_mod)
    {
        return -1;
    }

    if (log_buf_construct(&log_mod->buff, LOG_BUFF_ITEM_SIZE_MAX, LOG_BUFF_ITEM_FIX_LEN))
    {
        return -1;
    }
    strncpy(log_mod->log_file_path, __file_name[_mod], sizeof(log_mod->log_file_path));
    pthread_mutex_init(&log_mod->mutx, NULL);
    log_mod->log_level = log_level;
 
    if (0 == log2file_thread)
    {
        pthread_attr_t attr;

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
        pthread_create(&log2file_thread, &attr,_log2file, NULL);
    }

    return 1; 
}

stLogMod_t *_get_log_inst(int _mod)
{
    if (_mod >= LOG_MOD_MAX)
    {
        return NULL;
    }

    if(0 ==strlen(g_log_mod[_mod].log_file_path)) 
    {
        if (-1 == __log_module_init(g_log_mod + _mod, _mod, LOG_LEVEL_WARN))
        {
            return NULL;
        }
    }

    return g_log_mod + _mod;
}

int _check_log_level(stLogMod_t *log_inst,int level)
{
    if (level < log_inst->log_level)
    {
        return 0;
    }

    return 1;
}

int _logger_raw_nofmt(stLogMod_t *log_inst, int log_level, const char *_submod, const char *psmsg)
{
    struct timeval tv; 
    struct tm tm;
    char *szMsg = NULL;
    int szMsgLen = 0;

    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &tm);

    pthread_mutex_lock(&log_inst->mutx);
    szMsg = log_buff_allocate(&log_inst->buff);
    szMsgLen = snprintf(szMsg, log_inst->buff.fix_length, "%d-%02d-%02d %02d:%02d:%02d.%06ld [%s] %s %s\n", tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec, g_level2str[log_level], _submod, psmsg);
    if (szMsgLen == log_inst->buff.fix_length)
    {
        szMsg[szMsgLen - 1] = '\0';
    }
    pthread_mutex_unlock(&log_inst->mutx);

    return szMsgLen;

}

int loggernofmt(int _mod, int log_level, const char *_submod, const char *psmsg)
{
    stLogMod_t *log_inst = NULL;

    log_inst =_get_log_inst(_mod);
    if (NULL == log_inst)
    {
        return 0;
    }

    if(!_check_log_level(log_inst, log_level))
    {
        return 0;
    }

    return _logger_raw_nofmt(log_inst,log_level, _submod, psmsg);
}

int logger(int _mod, int level, const char *_submod,const char *fmt, ...)
{
    stLogMod_t *log_mod_inst  = NULL;
    log_mod_inst = _get_log_inst(_mod);
    if(NULL == log_mod_inst)
    {
        return 0;
    }
    if(!_check_log_level(log_mod_inst, level))
    {
        return 0;
    }

    char psMsg[256] = {0};
    va_list arg_list;
    va_start(arg_list, fmt);
    vsnprintf(psMsg, sizeof(psMsg), fmt, arg_list);
    va_end(arg_list);

    return _logger_raw_nofmt(log_mod_inst, level, _submod, psMsg);
}

void *_log2file(void *arg)
{
    int idx = 0;
    size_t item = 0;
    char *str_cache = NULL;
    while(1)
    {
        sleep(1);
        for(idx = 0;idx < LOG_MOD_MAX;idx++)
        {
            if(strlen(g_log_mod[idx].log_file_path))
            {
                pthread_mutex_lock(&g_log_mod[idx].mutx);
                item = log_buff_copy2cache(&g_log_mod[idx].buff, &str_cache);
                log_buff_reset(&g_log_mod[idx].buff);
                pthread_mutex_unlock(&g_log_mod[idx].mutx);
                if(item)
                {
                    log_file_cache2save(str_cache, item, g_log_mod[idx].buff.fix_length, g_log_mod[idx].log_file_path);
                    free(str_cache);
                }
            }
            str_cache = NULL;
            item = 0;
        }
    }
}

