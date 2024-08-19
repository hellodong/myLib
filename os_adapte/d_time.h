

#ifndef _D_TIME_H_
#define _D_TIME_H_

#include <stdint.h>

#ifdef __cplusplus 
extern "C" {
#endif

#define DTIME_BASED_YEAR	(2020)

typedef struct
{
	uint32_t d_year : 6;
	uint32_t d_mon : 4;
	uint32_t d_day : 5;
	uint32_t d_h : 5;
	uint32_t d_m : 6;
	uint32_t d_s : 6;
}DTIME_t;

uint32_t d_time_stampms(void);
uint32_t d_time_getpaktime(void);

#ifdef __cplusplus 
}
#endif

#endif