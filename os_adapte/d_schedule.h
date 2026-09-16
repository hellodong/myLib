

#ifndef D_TIMER_H
#define D_TIMER_H

#include <stdint.h>

typedef void (*TIMER_PROC_FUNC_t)(void *para, uint32_t nInterval);

int timer_init(void);

int timer_api_create(const char *name, TIMER_PROC_FUNC_t proc_func,uint32_t ninterval, void *para);

int timer_api_update_interval(const char *name, uint32_t interval);

int timer_api_delete(const char *name);

#endif
