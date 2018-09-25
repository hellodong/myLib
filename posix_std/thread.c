

#include "thread.h"


/*
 * retval: 0 -- existed, 1 -- no existed, 
 *	   	   2 -- illegal operation
 */
int thread_check(pthread_t tid) 
{
    int pthread_kill_err = 0;
	int ret = 0;


    pthread_kill_err = pthread_kill(tid,0);
    if(pthread_kill_err == ESRCH){
		ret = 1;
	}else if(pthread_kill_err == EINVAL){
        ret = 2;
    }else{
        ret = 0;
	}

    return ret;
}