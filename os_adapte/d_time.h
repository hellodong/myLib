

#ifndef _D_TIME_H_
#define _D_TIME_H_

#include <stdint.h>

#ifdef __cplusplus 
extern "C" {
#endif

#define DTIME_BASED_YEAR	(2020)

typedef struct {
    uint32_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t second;
}DTIMEStruct_t;

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

#define D_PAKTIME2TIME(pwd,d) \
    do {\
        DTIME_t *ptr=(DTIME_t *)pwd;\
        (d)->year=(uint32_t)(ptr->d_year + DTIME_BASED_YEAR); (d)->month = (uint8_t)(ptr->d_mon); (d)->day = (uint8_t)(ptr->d_day);\
        (d)->hour=(uint8_t)(ptr->d_h);(d)->min=(uint8_t)(ptr->d_m);(d)->second=(uint8_t)(ptr->d_s);\
    }while(0)

#ifdef __cplusplus 
}
#endif

#endif
