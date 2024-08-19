

#include "d_time.h"
#include <windows.h>

uint32_t d_time_stampms(void)
{
	LARGE_INTEGER time_now;
	LARGE_INTEGER freq;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&time_now);

	return uint32_t(time_now.QuadPart * 1000 / freq.QuadPart);
}

uint32_t d_time_getpaktime(void)
{
	SYSTEMTIME sys;
	DTIME_t d_time;
	uint32_t* ptr = (uint32_t *) & d_time;
	GetLocalTime(&sys);
	d_time.d_year = sys.wYear - DTIME_BASED_YEAR;
	d_time.d_mon = sys.wMonth;
	d_time.d_day = sys.wDay;
	d_time.d_h = sys.wHour;
	d_time.d_m = sys.wMinute;
	d_time.d_s = sys.wSecond;

	return *ptr;
}