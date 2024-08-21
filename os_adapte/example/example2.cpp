

#include "d_time.h"
#include "d_thread.h"
#include "d_semaphore.h"

#include <iostream>

DSem_t hSemMain;
DSem_t hSem1;

void thread1(DPvoid_t data)
{
	std::cout << "thread 1 semaphore test start" << std::endl;
	while (1)
	{
        d_sem_wait(hSemMain, 1);
		std::cout << d_time_stampms() << ": receive main" << std::endl;
        d_sem_post(hSem1);
		//d_thread_sleep(1);
	}
}

int main(int argc, char *argv[])
{
    hSemMain = d_sem_new(0);
    hSem1 = d_sem_new(0);
	DThread_t thread1id = d_thread_new(NULL, D_THREAD_PRI_IDLE, 256 * 1024, thread1, NULL);

    std::cout << __func__ <<" timestamp: "  << d_time_stampms() << thread1id <<std::endl;
    
    while(1)
    {
        d_sem_post(hSemMain);
        d_sem_wait(hSem1, 1);
        std::cout << d_time_stampms() << ": receive thread1" << std::endl;
    }

    return 0;
}
