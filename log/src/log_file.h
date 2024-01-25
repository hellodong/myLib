#ifndef _LOG_FILE_H_
#define _LOG_FILE_H_


#include <stddef.h>
#include <semaphore.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct {
    char *queue_str;
    int item;
    int fix_length;
    size_t cur_file_size;
    size_t max_file_size;
    char file_name[256];
    sem_t sem;
}stLogFile_t;

void log_file_init(stLogFile_t *log_file, const char *file_name, size_t max_file_size);

size_t log_file_set_cache(stLogFile_t *log_file, char *ring_str, size_t item_size, int fix_len);

size_t log_file_cache2save(stLogFile_t *log_file);

#ifdef __cplusplus
}
#endif

#endif 
