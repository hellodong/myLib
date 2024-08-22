

#ifndef _D_THREAD_H_
#define _D_THREAD_H_

#include<stdint.h>

#ifdef __linux__
#include "d_posix_thread.h"
#elif _WIN32
#include "d_win_thread.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define D_THREAD_STACKSIZE_128K		(128 * 1024)
#define D_THREAD_STACKSIZE_256K		(256 * 1024)
#define D_THREAD_STACKSIZE_512K		(512 * 1024)
#define D_THREAD_STACKSIZE_1M		(1024 * 1024)
#define D_THREAD_STACKSIZE_2M		(2048 * 1024)

typedef void (*DThreadTask_t)(void *arg);


DThread_t d_thread_new(char threadname[], int thread_pri, size_t stack_size, DThreadTask_t threadfunc, DPvoid_t parameter);

int d_thread_cancel(DThread_t hThread);

DThread_t d_thread_self(void);

void d_thread_sleep(uint32_t milisecond);

#ifdef __cplusplus
}
#endif

#endif
