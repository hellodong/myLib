
#ifndef _FIFO_H_
#define _FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif


struct _fifo_t{
    unsigned char *buffer;
    unsigned int size;
    unsigned int in;
    unsigned int out; 
};


int fifo_init(struct _fifo_t *fifo, unsigned char _buffer[], unsigned int size);

unsigned int fifo_put(struct _fifo_t *fifo, char data_buffer[], unsigned int len);
unsigned int fifo_get(struct _fifo_t *fifo, char data_buffer[], unsigned int size);

#ifdef __cplusplus
}
#endif

#endif
