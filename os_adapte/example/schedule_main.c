

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
    const char str_array[40][64] = {
        "schedule_task1", 
        "schedule_task2",
        "schedule_task3",
        "schedule_task4",
        "schedule_task5",
        "schedule_task6",
        "schedule_task7",
        "schedule_task8",
        "schedule_task9",
        "schedule_task10",
        "schedule_task11",
        "schedule_task12",
        "schedule_task13",
        "schedule_task14",
        "schedule_task15",
        "schedule_task16",
        "schedule_task17",
        "schedule_task18",
        "schedule_task19",
        "schedule_task20",
        "schedule_task21",
        "schedule_task22",
        "schedule_task23",
        "schedule_task24",
        "schedule_task25",
        "schedule_task26",
        "schedule_task27",
        "schedule_task28",
        "schedule_task29",
        "schedule_task30",
        "schedule_task31",
        "schedule_task32",
        "schedule_task33",
        "schedule_task34",
        "schedule_task35",
        "schedule_task36",
        "schedule_task37",
        "schedule_task38",
        "schedule_task39",
        "schedule_task40",
    };

    timer_init();

    printf("%u create %s\n", d_time_stampms(), str_array[0]);
    timer_api_create(str_array[0], schedule_task, 500, (void *)str_array[0]);
    d_thread_sleep(600);
    printf("%u update %s interval to 1\n", d_time_stampms(), str_array[0]);
    timer_api_update_interval(str_array[0], 1);
    d_thread_sleep(4);
    timer_api_delete(str_array[0]);
    printf("%u delete %s\n", d_time_stampms(), str_array[0]);
    d_thread_sleep(4);

    printf("%u create %s\n", d_time_stampms(), str_array[1]);
    timer_api_create(str_array[1], schedule_task, 100, (void *)str_array[1]);
    d_thread_sleep(300);
    timer_api_delete(str_array[1]);

    for (uint32_t idx = 0;idx < 40;idx++)
    {
        printf("%u create %s\n", d_time_stampms(), str_array[idx]);
        timer_api_create(str_array[idx], schedule_task, idx, (void *)str_array[idx]);
    }

    d_thread_sleep(20000);

    for (uint32_t idx = 0;idx < 40;idx++)
    {
        printf("%u delete %s\n", d_time_stampms(), str_array[idx]);
        timer_api_delete(str_array[idx]);
    }

    d_thread_sleep(100);

    return 0;
}