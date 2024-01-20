#ifndef _LOG_FILE_H_
#define _LOG_FILE_H_


#include <stddef.h>

size_t log_file_cache2save(const char *ring_str, size_t item_size, int fix_len, const char *file_name);

#endif 
