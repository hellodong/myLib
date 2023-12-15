

#include "log_file.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>


int g_maxfile_size = 2 * 1024 * 1024;
int g_log_level = 0;
int g_print = 0;
char g_log_file_name[256] = "";
unsigned char g_log_fine_index = 0;
const char *g_level2str[] = {
    "debug",
    "info",
    "warn",
    "error",
    "fatal",
};


int log_file_init(const char *path, int max_file_size, int level, int is_print)
{
    if (max_file_size > g_maxfile_size)
    {
        g_maxfile_size = max_file_size;
    }

    strncpy(g_log_file_name, path, sizeof(g_log_file_name));

    g_log_level = level;
    if (level > LOG_LEVEL_FATAL)
    {
        g_log_level = LOG_LEVEL_FATAL;
    }

    g_print = 0;
    if (is_print)
    {
        g_print=1;
    }
	return 0;
}

void _log_iterate(void)
{
    char file_new_name[256];
    char file_old_name[256];
    int file_idx = 9;

    snprintf(file_new_name, sizeof(file_new_name),"%s.%d.gz", g_log_file_name, file_idx);
    if (0 == access(file_new_name, F_OK)) {
        remove(file_new_name);
    }

    for (;file_idx > 0;file_idx--) {
        snprintf(file_new_name, sizeof(file_new_name, "%s.%d.gz"), g_log_file_name, file_idx);
        snprintf(file_old_name, sizeof(file_new_name, "%s.%d.gz"), g_log_file_name, file_idx - 1);
        if (0 == access(file_old_name, F_OK)) {
            rename(file_old_name, file_new_name);
        }
    }

    snprintf(file_new_name, sizeof(file_new_name), "%s.%d", g_log_file_name, file_idx);

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "gzip %s", file_new_name);
    system(cmd);
}

void _update_log_file(const char *file_name)
{
    char file_new_name[256];
    snprintf(file_new_name, sizeof(file_new_name),"%s.0", file_name);
    rename(file_name, file_new_name);

    pthread_t pid=0;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_create(&pid, &attr, _log_iterate, NULL);
}

int logFileWrite(const char *fun_name,int line, int level, const char *format, ...)
{
    int file_size = 0;
    int szMsgLen = 0;
    char szMsg[512];
    struct timeval tv; 
    struct tm tm;

    va_list arg_list;
    char buff[256];
    va_start(arg_list, format);
    vsnprintf(buff, sizeof(buff), format, arg_list);
    va_end(arg_list);

    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &tm);
    szMsgLen = snprintf(szMsg, sizeof(szMsg),"%d-%d-%d %d:%d:%d.%06ld %s-%s%d: %s\n",tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec, 
                        g_level2str[level], fun_name, line, buff);

    szMsg[sizeof(szMsg) - 1] = '\0';
    if (szMsgLen + 2 >= sizeof(szMsg)) {
        szMsg[sizeof(szMsg) - 2] = '\n';
    } else {
        szMsg[szMsgLen] = '\n';
        szMsg[szMsgLen+1] = 0;
    }

    if (g_print) {
        printf(szMsg);
    }

	FILE *fd = fopen(g_log_file_name, "a+");
	if (NULL == fd) {
        fprintf(stderr, "can't open %s\n", g_log_file_name);
		return 0;
	}
    fseek(fd, 0, SEEK_END);
    fwrite(szMsg, szMsgLen, 1, fd);
    file_size = ftell(fd);
	fclose(fd);

    if (file_size > g_maxfile_size) {
        _update_log_file(g_log_file_name);
    }
	return szMsgLen;
}

