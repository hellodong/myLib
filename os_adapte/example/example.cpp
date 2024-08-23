

#include <iostream>

#include "d_time.h"
#include "d_thread.h"
#include "d_mutex.h"
#include "d_semaphore.h"

int g_sem_lock_value = 0;
int g_mutex_lock_value = 0;
DSem_t hSem;
DMutex_t hMutex;

void thread1(DPvoid_t data)
{
	std::cout << "thread 1 semaphore test start" << std::endl;
	while (1)
	{
		d_thread_sleep(1000);
		d_sem_wait(hSem, DWAITFOREVER);
		std::cout << "thread1 sem lock value " << g_sem_lock_value << std::endl;
		g_sem_lock_value++;
		d_sem_post(hSem);
	}
}

void thread2mtx(DPvoid_t data)
{
	DMutex_t local_mtx =(DMutex_t )data;
	std::cout <<"thread mutex start " << std::endl;
	while(1)
	{
	    d_mutex_lock(local_mtx, -1);
            std::cout << "thread( "<<d_thread_self()<< ")mutex lock value "  << g_mutex_lock_value++ << std::endl;
	    d_mutex_unlock(local_mtx);
	    d_thread_sleep(1500);
	}
}

int main(int argc, char *argv[])
{
	hSem = d_sem_new(1);

	DThread_t thread1id = d_thread_new(NULL, D_THREAD_PRI_IDLE, 256 * 1024, thread1, NULL);
	hMutex = d_mutex_new();
	DThread_t thread2id = d_thread_new(NULL, D_THREAD_PRI_IDLE, 128 * 1024, thread2mtx, hMutex);
	DThread_t thread3id = d_thread_new(NULL, D_THREAD_PRI_IDLE, 128 * 1024, thread2mtx, hMutex);

	while (1)
	{
		d_thread_sleep(1000);
		d_sem_wait(hSem, DWAITFOREVER);
		std::cout << "main sem lock value " << g_sem_lock_value << std::endl;
		g_sem_lock_value++;
		d_sem_post(hSem);
	}
	return 0;
}
