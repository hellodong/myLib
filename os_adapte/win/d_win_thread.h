

#ifndef _D_WIN_THREAD_H_
#define _D_WIN_THREAD_H_

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define D_THREAD_PRI_IDLE			(THREAD_PRIORITY_IDLE)
#define D_THREAD_PRI_HIGHEST		(THREAD_PRIORITY_HIGHEST)
#define D_THREAD_PRI_HIGH			(THREAD_PRIORITY_ABOVE_NORMAL)
#define D_THREAD_PRI_NORMAL			(THREAD_PRIORITY_NORMAL)
#define D_THREAD_PRI_LOW			(THREAD_PRIORITY_BELOW_NORMAL)
#define D_THREAD_PRI_LOWEST			(THREAD_PRIORITY_LOWEST)

typedef HANDLE DThread_t;
typedef LPVOID DPvoid_t;



#ifdef __cplusplus
}
#endif

#endif