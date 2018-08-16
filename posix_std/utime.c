
#include <time.h>
#include <stdint.h>


uint32_t getUTCTime(void)
{
	return time(NULL);
}
