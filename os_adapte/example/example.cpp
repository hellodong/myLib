

#include <windows.h>
#include <iostream>

#include "d_time.h"
#include "d_thread.h"
#include "d_semaphore.h"

int g_sem_lock_value = 0;
DSem_t hSem;

void thread1(DPvoid_t data)
{
	std::cout << "thread 1 semaphore test start" << std::endl;
	while (1)
	{
		d_thread_sleep(1000);
		d_sem_wait(hSem, 0);
		std::cout << "thread1 sem lock value " << g_sem_lock_value << std::endl;
		g_sem_lock_value++;
		d_sem_post(hSem);
	}
}

int main(int argc, char *argv[])
{
	hSem = d_sem_new(0);
	DThread_t thread1id = d_thread_new(NULL, D_THREAD_PRI_LOW, 256 * 1024, thread1, NULL);

	d_sem_post(hSem);
	while (1)
	{
		d_thread_sleep(1000);
		d_sem_wait(hSem, 0);
		std::cout << "main sem lock value " << g_sem_lock_value << std::endl;
		g_sem_lock_value++;
		d_sem_post(hSem);
	}
	return 0;
}