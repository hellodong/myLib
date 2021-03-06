

#ifndef _CONDITION_H_
#define _CONDITION_H_


#include <stdint.h>

struct _condition_t
{
    pthread_cond_t  cond;
    pthread_mutex_t mutex;
    int code;
};

typedef struct _condition_t Cond_t;


int condInit(Cond_t *cond);

int condWait(Cond_t *cond, uint32_t waitSecs);

int condTrigger(Cond_t *cond);


#endif
