

#include "log_buf.h"

#include <string.h>
#include <stdlib.h>


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
    int actPos = 0;
    
    actPos = log_buff->free_pos % log_buff->item_size;
    log_buff->free_pos++;
    if (log_buff->free_pos > log_buff->item_size)
    {
        log_buff->cur_head++;
    }
    return log_buff->ringbuf + actPos * log_buff->fix_length;
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
    if (NULL == log_buff)
    {
        return;
    }
    log_buff->cur_head = 0;
    log_buff->free_pos = 0;
}

size_t log_buff_copy2cache(stLogBuff_t *log_buff, char **str_cache)
{
    size_t all_items = 0, idx = 0;
    size_t buff_idx = 0;
    if (NULL == log_buff 
       || NULL == str_cache 
       || 0 == log_buff->free_pos)
    {
        return 0;
    }
    
    all_items = log_buff->free_pos > log_buff->item_size ? log_buff->item_size :log_buff->free_pos;
    *str_cache = (char *) malloc (all_items * log_buff->fix_length);
    if (NULL == str_cache)
    {
        return 0;
    }

    for(idx = 0;idx < all_items;idx++)
    {
        buff_idx =  (log_buff->cur_head + idx) % log_buff->item_size * log_buff->fix_length;
        memcpy(str_cache + idx * log_buff->fix_length, log_buff->ringbuf + buff_idx, log_buff->fix_length);
    }

    return all_items;
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

