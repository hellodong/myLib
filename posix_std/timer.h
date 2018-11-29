

#ifndef TIMER_H
#define TIMER_H


typedef void (*stCallbackFunc_t)(void *arg);


int timer_add(stCallbackFunc_t func, unsigned short interval_ms, void *arg, unsigned char sleepy);

void timer_del(int timerId);

int timer_sleepyisDone(void);

void timer_run(void);


#endif
