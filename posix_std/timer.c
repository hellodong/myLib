

#include <stdint.h>
#include <sys/time.h>
#include <pthread.h>
#include "timer.h"


#define ARRAY_MAX_NUM		(16)


typedef struct
{
	uint8_t availid;
	uint32_t interval;
	uint64_t setUpTime_ms;
	void *arg;
	stCallbackFunc_t func;
}stTimer_t;


static  stTimer_t timerArray[ARRAY_MAX_NUM];
static pthread_mutex_t gMtx;


static uint64_t get_time_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, 0);
	return tv.tv_sec  * 1000 + tv.tv_usec / 1000;
}

static uint8_t elapsed(uint64_t startTime, uint64_t nowTime, uint32_t intval)
{
	uint64_t reachTime = startTime + intval;
	if (reachTime >= startTime){
		if (nowTime >= reachTime){
			return 1;
		}else{
			return 0;
		}
	}else{
		if (nowTime + intval >= reachTime + intval){
			return 1;
		}else{
			return 0;
		}
	}
}

int timer_init(void)
{
	pthread_mutex_init(&gMtx, NULL);		
	return 0;
}

int timer_add(stCallbackFunc_t func, void *arg, uint32_t interval_ms)
{
	unsigned char idx;
	unsigned ret = -1;

	pthread_mutex_lock(&gMtx);
	for(idx = 0;idx < ARRAY_MAX_NUM;idx++) {
		if(!timerArray[idx].availid) {
			timerArray[idx].setUpTime_ms = get_time_ms();
			timerArray[idx].func = func;
			timerArray[idx].arg = arg;
			timerArray[idx].interval = interval_ms;
			timerArray[idx].availid = 1;
			ret = idx;
			break;
		}
	}
	pthread_mutex_unlock(&gMtx);
	return ret;
}

void timer_del(int timerId)
{
	if (timerId < 0 || timerId >= ARRAY_MAX_NUM) {
		return;
	}
	pthread_mutex_lock(&gMtx);
	timerArray[timerId].availid = 0;
	pthread_mutex_unlock(&gMtx);
}

void timer_run(void)
{
	int idx;
	stCallbackFunc_t func;
	void *arg;
	uint8_t availid;
	for(idx = 0;idx < ARRAY_MAX_NUM;idx++) {
		pthread_mutex_lock(&gMtx);
		availid = timerArray[idx].availid;
		if (availid){
			if (elapsed(timerArray[idx].setUpTime_ms, get_time_ms(), timerArray[idx].interval)){
				timerArray[idx].setUpTime_ms = get_time_ms();
				arg = timerArray[idx].arg;
				func = timerArray[idx].func;
			}else{
				func = NULL;
			}
		}else{
			func = NULL;
		}
		pthread_mutex_unlock(&gMtx);
		if(func){
			func(arg);
		}
	}
}

