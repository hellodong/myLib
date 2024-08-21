

#include "d_semaphore.h"


DSem_t d_sem_new(uint32_t init_val)
{
	return CreateSemaphore(NULL, init_val, 65535,NULL);
}

int d_sem_wait(DSem_t hSem, uint32_t time_out)
{
	DWORD dwMiliSeconds = time_out == DWAITFOREVER? INFINITE : time_out;

	return WaitForSingleObject(hSem, dwMiliSeconds);
}

int d_sem_post(DSem_t hSem)
{
	if (ReleaseSemaphore(hSem, 1, NULL))
	{
		return 0;
	}
	return -1;
}

int d_sem_destroy(DSem_t hSem)
{
	CloseHandle(hSem);

	return 0;
}
