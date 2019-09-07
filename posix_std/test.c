

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "condition.h"
#include "utime.h"

#define LOG(fmt,arg...)   printf("%s() #%d "fmt"\n", __func__, __LINE__, ##arg)


//Cond_t gCond;


void *test_thread(void *arg);


int main (void)
{
    int ret;
	uint32_t now;
    pthread_t threadId;

    //condInit(&gCond); 
    //pthread(&threadId, NULL, test_thread, NULL);
    for(;;)
    {
        //ret = condWait(&gCond, 10);
		now = getUTCTime();
        LOG("time:%d", now);
		sleep(1);
    }
    return 0;
}

void *test_thread(void *arg)
{
    for(;;)
    {
        //condTrigger(&gCond);
        sleep(5);
    }
}

