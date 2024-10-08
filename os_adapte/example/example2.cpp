

#include "d_time.h"
#include "d_thread.h"
#include "d_semaphore.h"

#include <iostream>
#include <iomanip>

DSem_t hSemMain;
DSem_t hSem1;

void thread1(DPvoid_t data)
{
	std::cout << "thread 1 semaphore test start" << std::endl;
	while (1)
	{
        d_sem_wait(hSemMain, DWAITFOREVER);
		std::cout << d_time_stampms() << ": receive main" << std::endl;
        d_sem_post(hSem1);
	}
}

int main(int argc, char *argv[])
{
    hSemMain = d_sem_new(0);
    hSem1 = d_sem_new(0);
	DThread_t thread1id = d_thread_new(NULL, D_THREAD_PRI_IDLE, 256 * 1024, thread1, NULL);

    std::cout << __func__ <<" timestamp: "  << d_time_stampms() <<" thread id:" <<thread1id <<std::endl;

    DTIMEStruct_t d_time = {0};
    uint32_t now_d_time = d_time_getpaktime();
    D_PAKTIME2TIME(&now_d_time,&d_time);
    std::cout <<d_time.year<<"-"<<std::setw(2)<<std::setfill('0')<<d_time.month + 0<<"-"<< std::setw(2)<<std::setfill('0')<< d_time.day + 0<<" "<< std::setw(2)<<std::setfill('0')<< d_time.hour+ 0<<":" << std::setw(2)<<std::setfill('0')  << d_time.min + 0<<":"<< std::setw(2)<<std::setfill('0')<< d_time.second + 0<<std::endl;
     
    while(1)
    {
        d_sem_post(hSemMain);
        d_sem_wait(hSem1, DWAITFOREVER);
        std::cout << d_time_stampms() << ": receive thread1" << std::endl;
    }

    return 0;
}
