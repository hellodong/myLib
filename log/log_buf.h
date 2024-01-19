

#ifndef _LOG_BUF_H_
#define _LOG_BUF_H_

#define LOG_BUFF_ITEM_SIZE_MAX  (16384)
#define LOG_BUFF_ITEM_FIX_LEN   (384)


typedef struct _log_buf
{
    char *ringbuf;
    int item_size;
    int fix_length;
    int cur_head;
    int free_pos;
}stLogBuff_t;

int log_buf_construct(stLogBuff_t *log_buff, int item_size, int fix_length);
int log_buff_get_size(stLogBuff_t *log_buff)
void log_buff_destruct(stLogBuff_t *log_buff);
void log_buff_reset(stLogBuff_t *log_buff);
char* log_buff_allocate(stLogBuff_t *log_buff);
char* log_buff_get_pos(stLogBuff_t *log_buff, int n_pos);

#endif
