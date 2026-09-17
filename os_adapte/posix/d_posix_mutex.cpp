

#include <stdlib.h>
#include <time.h>

#include "d_mutex.h"


DMutex_t d_mutex_new(void)
{
    pthread_mutex_t *pMtx = NULL;
    pMtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if (NULL == pMtx)
    {
        return pMtx;
    }

    int rt = pthread_mutex_init(pMtx, NULL);
    if (rt != 0)
    {
        free(pMtx);
        return NULL;
    }

    return pMtx;
}

int d_mutex_lock(DMutex_t mtx, unsigned int timeout)
{
    if(DWAITFOREVER == timeout)
    {
        return pthread_mutex_lock(mtx);
    }
    else if (DNOWAIT == timeout)
    {
        return pthread_mutex_trylock(mtx);
    }

    struct timespec ts;
    
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
    {
        return -1;
    }
    ts.tv_sec += timeout / 1000;
    ts.tv_nsec += (timeout % 1000) * 1000000;
    ts.tv_sec +=ts.tv_nsec / 1000000000;
    ts.tv_nsec %= 1000000000;

    return pthread_mutex_timedlock(mtx, &ts);
}

int d_mutex_trylock(DMutex_t mtx)
{
    return pthread_mutex_trylock(mtx);
}

int d_mutex_unlock(DMutex_t mtx)
{
    return pthread_mutex_unlock(mtx);
}

int d_mutex_destroy(DMutex_t mtx)
{
    pthread_mutex_destroy(mtx);
    free(mtx);
    return 0;
}

