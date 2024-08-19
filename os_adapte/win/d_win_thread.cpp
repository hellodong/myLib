
#include "d_thread.h"


DThread_t d_thread_new(char threadname[], int thread_pri, size_t stack_size, void* threadfunc, DPvoid_t parameter)
{
	DThread_t threadhandle;

	threadhandle = CreateThread(NULL, stack_size, (LPTHREAD_START_ROUTINE)threadfunc, parameter, 0, NULL);
	if (NULL == threadhandle)
	{
		return NULL;
	}

#if 0
	if (threadname)
	{
		SetThreadDescription(threadhandle, TEXT(threadname));
	}
	else
#endif
	{
		SetThreadDescription(threadhandle, L"null");
	}

	SetThreadPriority(threadhandle, thread_pri);

	return threadhandle;
}

int d_thread_cancel(DThread_t hThread)
{
	//Todo 
	return 0;
}

DThread_t d_thread_self(void)
{
	return GetCurrentThread();
}

void d_thread_sleep(uint32_t milisecond)
{
	Sleep(milisecond);
}

