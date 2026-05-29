

#include "fifo.h"
#include <string.h>

#define min(a,b) (a)<(b)?(a):(b)

int fifo_init(struct _fifo_t *fifo, unsigned char _buffer[], unsigned int size)
{
    if (NULL == _buffer || 
            (size & (size - 1)))
    {
        return -1;
    }
    fifo->buffer = _buffer;
    fifo->size = size;
    fifo->in = fifo->out = 0;

    return 0;
}

unsigned int fifo_put(struct _fifo_t *fifo, char data_buffer[], unsigned int len)
{
    unsigned int l = 0;
    unsigned int offset = 0;

    offset = fifo->in & (fifo->size - 1);
    len = min(len, fifo->size - fifo->in + fifo->out);
    l = min(len, fifo->size - offset);

    memcpy(fifo->buffer + offset, data_buffer, l);

    memcpy(fifo->buffer, data_buffer + l, len - l);

    fifo->in += len;

    return len;
}

unsigned int fifo_get(struct _fifo_t *fifo, char data_buffer[], unsigned int size)
{
    unsigned int l = 0;
    unsigned int len = 0;
    unsigned int offset = 0;

    offset = fifo->out & (fifo->size - 1);
    len = min(size, fifo->in - fifo->out);
  
    l = min(len, fifo->size - offset);
    memcpy(data_buffer, fifo->buffer + offset,  l);
    memcpy(data_buffer + l, fifo->buffer, len -l);

    fifo->out +=len;
    return len;
}
