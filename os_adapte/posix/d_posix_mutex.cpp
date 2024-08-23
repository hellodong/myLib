

#include <stdlib.h>

#include "d_mutex.h"

DMutex_t d_mutex_new(void)
{
    pthread_mutex_t *pMtx = NULL;
    pMtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if (NULL == pMtx)
    {
        return pMtx;
    }

    rt = pthread_mutex_init(pMtx, NULL);
    if (rt != 0)
    {
        free(pMtx);
        return NULL;
    }

    return pMtx;
}

int d_mutex_lock(DMutex_t mtx, int timeout)
{

}

int d_mutex_trylock(DMutex_t mtx);

int d_mutex_unlock(DMutex_t mtx);

int d_mutex_destroy(DMutex_t mtx);


