

#include "d_semaphore.h"

#include <time.h>
#include <stdlib.h>
#include <errno.h>


DSem_t d_sem_new(uint32_t init_val)
{
    sem_t *psem = NULL;
    psem = (sem_t*)malloc(sizeof(sem_t));
    int rt = 0;
    rt = sem_init(psem, 0, init_val);
    if (rt)
    {
        free(psem);
        return NULL;
    }

    return psem;
}

int d_sem_wait(DSem_t hSem, uint32_t time_out)
{
    int rt = -1;
    if (DNOWAIT == time_out)
    {
        rt = sem_trywait(hSem);
    }
    else if (DWAITFOREVER == time_out)
    {
        while(-1 == (rt = sem_wait(hSem)) && EINTR == errno);
    }
    else
    {
        struct timespec ts;
        if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
        {
            return -1;
        }
        ts.tv_sec += time_out / 1000;
        ts.tv_nsec += (time_out % 1000) * 1000000;
        ts.tv_sec +=ts.tv_nsec / 1000000000;
        ts.tv_nsec %= 1000000000;

        rt = sem_timedwait(hSem, &ts);
    }

    return rt;
}

int d_sem_post(DSem_t hSem)
{
    return sem_post(hSem);
}

int d_sem_destroy(DSem_t hSem)
{
    sem_destroy(hSem);
    free(hSem);
    return 0;
}


