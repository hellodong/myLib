
#ifndef _D_POSIX_THREAD_H_
#define _D_POSIX_THREAD_H_

#include <pthread.h>

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum {
    D_THREAD_PRI_IDLE,
    D_THREAD_PRI_LOWEST,
    D_THREAD_PRI_LOW,
    D_THREAD_PRI_NORMAL,
    D_THREAD_PRI_HIGH,
    D_THREAD_PRI_HIGHEST,
}DThread_pri_t;

typedef pthread_t DThread_t;

typedef void* DPvoid_t;

#ifdef __cplusplus
}
#endif

#endif //d_posix_thread.h
