

#ifndef _D_SEMAPHORE_H_
#define _D_SEMAPHORE_H_

#include <stdint.h>

#ifdef __linux__
#include "d_posix_sem.h"
#elif _WIN32
#include "d_win_sem.h"
#endif

#ifdef __cplusplus 
extern "C" {
#endif


DSem_t d_sem_new(uint32_t init_val);

int d_sem_wait(DSem_t hSem, uint32_t time_out);

int d_sem_post(DSem_t hSem);

int d_sem_destroy(DSem_t hSem);

#ifdef __cplusplus 
}
#endif

#endif