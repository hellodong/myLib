

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
#include <semaphore.h>


#define LOG_FILE_STR_MAX    (256)

#define LOG_MOD_IS_RUNNING(log,_mod)   (log & (1 << (_mod)))
#define LOG_MOD_SET_BIT(log,_mod)     (log |= 1 << (_mod))

typedef struct _logMod
{
    int log_level;
    pthread_mutex_t mutx;
    stLogBuff_t buff;
    stLogFile_t log_file;
}stLogMod_t;

static unsigned int g_start_mod = 0;
static sem_t sem;
static pthread_t log2file_thread;
static stLogMod_t g_log_mod[LOG_MOD_MAX];

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
    log_file_init(&log_mod->log_file, __file_name[_mod], 10 * 1024 * 1024);

    pthread_mutex_init(&log_mod->mutx, NULL);
    log_mod->log_level = log_level;
 
    if (!g_start_mod)
    {
        pthread_attr_t attr;

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
        pthread_create(&log2file_thread, &attr,_log2file, NULL);
        sem_init(&sem, 0, 0);
    }


    return 1; 
}

stLogMod_t *_get_log_inst(int _mod)
{
    if (_mod >= LOG_MOD_MAX)
    {
        return NULL;
    }

    if(!LOG_MOD_IS_RUNNING(g_start_mod, _mod)) 
    {
        if (-1 == __log_module_init(g_log_mod + _mod, _mod, LOG_LEVEL_WARN))
        {
            return NULL;
        }
        LOG_MOD_SET_BIT(g_start_mod, _mod);
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
    size_t free_pos = 0;

    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &tm);

    pthread_mutex_lock(&log_inst->mutx);
    szMsg = log_buff_allocate(&log_inst->buff);
    szMsgLen = snprintf(szMsg, log_inst->buff.fix_length, "%d-%02d-%02d %02d:%02d:%02d.%06ld [%s] %s %s\n", tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec, g_level2str[log_level], _submod, psmsg);
    if (szMsgLen == log_inst->buff.fix_length)
    {
        szMsg[szMsgLen - 1] = '\0';
    }
    free_pos = log_inst->buff.free_pos;
    pthread_mutex_unlock(&log_inst->mutx);

    if(free_pos)
    {
        sem_post(&sem);
    }

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
    size_t item = 0, fix_len = 0;
    char *str_cache = NULL;
    while(1)
    {
        //sleep(1);
        sem_wait(&sem);
        usleep(10000);
        for(idx = 0;idx < LOG_MOD_MAX;idx++)
        {
            if (LOG_MOD_IS_RUNNING(g_start_mod, idx))
            {
                pthread_mutex_lock(&g_log_mod[idx].mutx);
                item = log_buff_copy2cache(&g_log_mod[idx].buff, &str_cache);
                fix_len = g_log_mod[idx].buff.fix_length;
                log_buff_reset(&g_log_mod[idx].buff);
                pthread_mutex_unlock(&g_log_mod[idx].mutx);
                if(item)
                {
                    log_file_set_cache(&g_log_mod[idx].log_file, str_cache, item, fix_len);
                    log_file_cache2save(&g_log_mod[idx].log_file);
                    log_file_set_cache(&g_log_mod[idx].log_file, NULL, 0, fix_len);
                    free(str_cache);
                }
            }
            str_cache = NULL;
            item = 0;
        }
    }
}

