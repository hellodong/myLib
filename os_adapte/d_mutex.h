
#ifndef _D_MUTEX_H_
#define _D_MUTEX_H_

#ifdef __linux__
#include "d_posix_mutex.h"
#elif _WIN32
#include "d_win_mutex.h"
#endif


#ifdef __cplusplus
extern "C"
{
#endif

typedef D_IMPL_Mutex_t DMutex_t;


DMutex_t d_mutex_new(void);

int d_mutex_lock(DMutex_t mtx, int timeout);

int d_mutex_trylock(DMutex_t mtx);

int d_mutex_unlock(DMutex_t mtx);

int d_mutex_destroy(DMutex_t mtx);

#ifdef __cplusplus
}
#endif

#endif //d_mutex.h
