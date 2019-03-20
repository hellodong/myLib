

#include "myRingQueue.h"

#include <stdio.h>

ringQueue_t ring;
uint8_t buffer[129];

int main (void)
{
	initQueue(&ring, buffer, sizeof(buffer)/sizeof(buffer[0]));

	int idx = 0;
	for (idx;idx < 200;idx++) {
		enQueue_stp(&ring, idx);
	}
	uint8_t data;
	while(popQueue(&ring, &data))
	{
		printf("%d\r\n", data);
	}
	return 0;
}
