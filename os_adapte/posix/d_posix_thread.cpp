

#include "d_thread.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>


typedef struct {
    char taskname[32];
    DThreadTask_t pfFunc;
    void *arg;
}threadPara_t;

int __d_priority_get(int pri)
{
    switch (pri)
    {
    case D_THREAD_PRI_LOWEST:
        return 1;
    case D_THREAD_PRI_LOW:
        return 15;
    case D_THREAD_PRI_NORMAL:
        return 20;
    case D_THREAD_PRI_HIGH:
        return 30;
    case D_THREAD_PRI_HIGHEST:
        return 50;
    default:
        return 1;
    }
}

static void* __task(void *arg)
{
    threadPara_t taskParam;

    memcpy(&taskParam, arg, sizeof(threadPara_t));
    free(arg);
    taskParam.pfFunc(taskParam.arg); 

    return NULL;
}

DThread_t d_thread_new(char threadname[], int thread_pri, size_t stack_size, DThreadTask_t threadfunc, DPvoid_t parameter)
{
    pthread_t thread_id = 0;
    struct sched_param schedpara = {0};
    pthread_attr_t attr;
    int schedpolicy = SCHED_OTHER;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if (thread_pri > 0)
    {
        schedpolicy = SCHED_RR;
        schedpara.sched_priority = __d_priority_get(thread_pri);
    }
    pthread_attr_setschedpolicy(&attr, schedpolicy);
    pthread_attr_setschedparam(&attr, &schedpara);

    stack_size = stack_size < D_THREAD_STACKSIZE_128K ? D_THREAD_STACKSIZE_128K : stack_size;
    pthread_attr_setstacksize(&attr, stack_size);

    threadPara_t *ptask_para;
    ptask_para = (threadPara_t *)malloc(sizeof(threadPara_t));

    ptask_para->pfFunc = threadfunc;
    if (parameter)
    {
        ptask_para->arg = parameter;
    }
    if (threadname)
    {
        strncpy(ptask_para->taskname, threadname, sizeof(ptask_para->taskname));
    }
    else
    {
        memcpy(ptask_para->taskname, "null", 5);
    }

    pthread_create(&thread_id, &attr, __task, (void *)ptask_para);
    return thread_id;
}

int d_thread_cancel(DThread_t hThread)
{
    return 0;
}

DThread_t d_thread_self(void)
{
    return pthread_self();
}

void d_thread_sleep(uint32_t milisecond)
{
    if (0 == milisecond)
    {
        sched_yield();
    }
    else
    {
        struct timeval tv = {0, 0};
        tv.tv_sec = milisecond / 1000;
        tv.tv_usec = milisecond % 1000 * 1000;
        select (0, NULL, NULL, NULL, &tv);
    }
}

