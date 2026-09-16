

#include <stdio.h>
#include <stdlib.h>

#include "d_schedule.h"
#include "d_thread.h"
#include "d_time.h"


void schedule_task(void *para, uint32_t nInterval)
{
    const char *str = (const char *)para;
    printf("%u %s, interval: %d\n", d_time_stampms(), str, nInterval);
}

int main(int argc, char *argv[])
{
    const char str_array[2][64] = {"schedule_task1", "schedule_task2"};

    timer_init();

    printf("%u create %s\n", d_time_stampms(), str_array[0]);
    timer_api_create(str_array[0], schedule_task, 500, (void *)str_array[0]);
    d_thread_sleep(600);
    timer_api_update_interval(str_array[0], 1);
    d_thread_sleep(4);
    timer_api_delete(str_array[0]);
    printf("%u delete %s\n", d_time_stampms(), str_array[0]);
    d_thread_sleep(4);

    printf("%u create %s\n", d_time_stampms(), str_array[1]);
    timer_api_create(str_array[1], schedule_task, 100, (void *)str_array[1]);
    d_thread_sleep(300);
    timer_api_delete(str_array[1]);

    return 0;
}