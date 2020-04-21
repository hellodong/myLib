

#ifndef TIMER_H
#define TIMER_H


typedef void (*stCallbackFunc_t)(void *arg);


int timer_init(void);

int timer_add(stCallbackFunc_t func, void *arg, unsigned int interval_ms);

void timer_del(int timerId);

void timer_run(void);


#endif
