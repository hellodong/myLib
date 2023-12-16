

#include "log_file.h"

#include <stdio.h>
#include <pthread.h>

void log_first_stage(int is_print)
{
    int idx = 0;
    for (idx = 0;idx < 0xfffff;idx++)
    {
        log_debug("%d", idx);
    }
}

void log_sec_stage(void)
{
    char buff[256];
    int szLen = 0;
    int idx = 0, jdx = 0;

    for (idx = 0; idx < 0xfff;idx++)
    {
        szLen = snprintf(buff, sizeof(buff),"%d", idx);
        for (jdx = 0; jdx < (sizeof(buff) - szLen - 1);jdx++)
        {

            buff[jdx] = '*';
        }
        snprintf(buff + jdx, sizeof(buff) - szLen, "%d", idx);
        log_info("%s", buff);
    }

}

void* __more_thread_log(void *arg)
{
    char buff[256];
    int szLen = 0;
    int idx = 0, jdx = 0;

    for (idx = 0; idx < 0xfff;idx++)
    {
        szLen = snprintf(buff, sizeof(buff),"0x%x", idx);
        for (jdx = 0; jdx < (sizeof(buff) - szLen - 1);jdx++)
        {

            buff[jdx] = '=';
        }
        snprintf(buff + jdx, sizeof(buff) - szLen, "0x%x", idx);
        log_warn("%s", buff);
    }
    return NULL;
}
void log_third_stage(pthread_t *pid)
{
    pthread_create(pid, NULL, __more_thread_log, NULL);
}

int main(int argc, char *argv[])
{
    pthread_t pid;
    log_file_init("./test.log", 1, LOG_LEVEL_INFO, 0);

    log_third_stage(&pid);

    log_sec_stage();

    pthread_join(pid, NULL);

    return 0;
}
