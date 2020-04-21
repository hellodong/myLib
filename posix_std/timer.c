

#include "sys/time.h"
#include "timer.h"


#define ARRAY_MAX_NUM		(64)


typedef struct
{
	unsigned char availid;
	unsigned short interval;
	unsigned int setUpTime_ms;
	void *arg;
	stCallbackFunc_t func;
}stTimer_t;

static  stTimer_t timerArray[ARRAY_MAX_NUM];


static unsigned int get_time_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, 0);
	return tv.tv_sec  * 1000 + tv.tv_usec / 1000;
}

int timer_add(stCallbackFunc_t func, unsigned short interval_ms, void *arg)
{
	unsigned char idx;
	for(idx = 0;idx < ARRAY_MAX_NUM;idx++) {
		if(!timerArray[idx].availid) {
			timerArray[idx].setUpTime_ms = get_time_ms();
			timerArray[idx].func = func;
			timerArray[idx].arg = arg;
			timerArray[idx].interval = interval_ms;
			timerArray[idx].availid = 1;
			return idx;
		}
	}
	return -1;
}

void timer_del(int timerId)
{
	if (timerId < 0 || timerId >= ARRAY_MAX_NUM) {
		return;
	}
	timerArray[timerId].availid = 0;
}

void timer_run(void)
{
	int idx;
	for(idx = 0;idx < ARRAY_MAX_NUM;idx++) {
		if(timerArray[idx].availid){
			unsigned int reatchTime = timerArray[idx].setUpTime_ms + timerArray[idx].interval;
			if (reatchTime <= get_time_ms()){
				stCallbackFunc_t func = timerArray[idx].func;
				func(timerArray[idx].arg);
				timerArray[idx].availid = 0;
			}
		}
	}
}

