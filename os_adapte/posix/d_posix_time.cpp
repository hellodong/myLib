
#include "d_time.h"

#include <time.h>
#include <sys/time.h>
#include <unistd.h>


uint32_t d_time_stampms(void)
{
    struct timespec sp = {0,0};

    clock_gettime(CLOCK_MONOTONIC, &sp);
    return (uint32_t)(sp.tv_sec * 1000 + sp.tv_nsec / 1000000);
}

uint32_t d_time_getpaktime(void)
{
    struct timeval tv = {0,0};
    struct tm l_time;
	DTIME_t d_time;
	uint32_t *ptr = (uint32_t *)&d_time;

    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &l_time);

	d_time.d_year = l_time.tm_year - 120;
	d_time.d_mon = l_time.tm_mon + 1;
	d_time.d_day = l_time.tm_mday;
	d_time.d_h = l_time.tm_hour;
	d_time.d_m = l_time.tm_min;
	d_time.d_s = l_time.tm_sec;

    return *ptr;
}
