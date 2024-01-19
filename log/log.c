

#include "log.h"

#include <string.h>
#include <pthread.h>


#define LOG_FILE_STR_MAX    (256)

typedef struct _logMod
{
    char log_file_path[LOG_FILE_STR_MAX];
    pthread_mutex_t mutx;
    stLogBuff_t buff;

}stLogMod_t;

pthread_t log2file_thread;
stLogMod_t g_log_mod[LOG_MOD_MAX];


static int __log_module_init(stLog_Mod_t *log_mod, int _mod)
{
    const char __file_name[LOG_MOD_MAX] = LOG_MOD_FILE_PATH;
    if (NULL == log_mod)
    {
        return -1;
    }

    if (log_buf_construct(g_log_mod, LOG_BUFF_ITEM_SIZE_MAX, LOG_BUFF_ITEM_FIX_LEN))
    {
        return -1;
    }
    strncpy(log_mod->log_file_path, __file_name[_mod], sizeof(log_mod->log_file_path));
    pthread_mutex_init(&log_mod->mutx, NULL);

    return 1; 
}

int loggernofmt(int _mod, const char *_submod, const char *psmsg)
{
    if (_mod >= LOG_MOD_MAX)
    {
        return -1;
    }
    if(NULL ==g_log_mode[mod].log_file_path) 
    {
        __log_module_init(g_log_mod + _mod, _mod);
    }
}

int logger(int _mod, const char *_submod, ...)
{
}
