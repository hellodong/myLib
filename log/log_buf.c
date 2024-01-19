

#include "log_buf.h"


int log_buf_construct(stLogBuff_t *log_buff, int item_size, int fix_length)
{
    if (NULL == log_buff)
    {
        return -1;
    }

    log_buff->item_size = item_size;
    log_buff->fix_length = fix_length;
    log_buff->ringbuf = (char *)malloc(item_size * fix_length);
    if(NULL == log_buff->ringbuf)
    {
        return -1;
    }
    log_buff->cur_head = 0;
    log_buff->free_pos = 0;

    return 0;
}

char* log_buff_allocate(stLogBuff_t *log_buff)
{
    int active = 0;
    
    active = log_buff->free_pos % item_size;
    log_buff->free_pos++;
    if (log_buff->free_pos > log_buff->item_size)
    {
        log_buff->cur_head++;
    }
    return log_buff->ringbuf + active * log_buff->fix_length;
}

char *log_buff_get_pos(stLogBuff_t *log_buff, int n_pos)
{
    if (n_pos > log_buff->item_size)
    {
        return NULL;
    }

    return log_buff->ringbuf + (n_pos + log_buff->cur_head) % log_buff->item_size * log_buff->fix_length;
}

void log_buff_reset(stLogBuff_t *log_buff)
{
    log_buff->cur_head = 0;
    log_buff->free_pos = 0;
}

int log_buff_get_size(stLogBuff_t *log_buff)
{
    if (log_buff->free_pos > log_buff->item_size)
    {
        return log_buff->item_size;
    }

    return log_buff->free_pos;
}

void log_buff_destruct(stLogBuff_t *log_buff)
{
    if (NULL == log_buff)
    {
        return;
    }
    free(log_buff->ringbuf);
    memset(log_buff, 0x00, sizeof(stLogBuff_t));
}

