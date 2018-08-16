

#include <stdint.h>
#include <sys/time.h>
#include <pthread.h>

#include "condition.h"


int condInit(Cond_t *cond)
{
    pthread_cond_init(&cond->cond, NULL);
    pthread_mutex_init(&cond->mutex, NULL);
    cond->code = 0;
    return 0;
}

int condWait(Cond_t *cond, uint32_t waitSecs)
{
    struct timeval now;
    struct timespec outtime;
    int ret;

    pthread_mutex_lock(&cond->mutex);
    gettimeofday(&now, NULL);
    outtime.tv_sec = now.tv_sec + waitSecs;
    outtime.tv_nsec = now.tv_usec * 1000;
    ret = pthread_cond_timedwait(&cond->cond, &cond->mutex, &outtime);
    pthread_mutex_unlock(&cond->mutex);
    return ret;
}

int condTrigger(Cond_t *cond)
{
    pthread_mutex_lock(&cond->mutex);
    pthread_cond_signal(&cond->cond);
    pthread_mutex_unlock(&cond->mutex);
    return 0;
}

