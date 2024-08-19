

#include "log_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


#define LOG_FILE_SIZE_MIN	(16 * 1024)


void *_log_iterate(void *arg)
{
    stLogFile_t *log_file = (stLogFile_t *)arg;
    char file_name[2][128];
    int file_idx = 9;

    snprintf(file_name[file_idx & 0x01], sizeof(file_name[0]),"%s.%d.gz", log_file->file_name, file_idx);
    if (0 == access(file_name[file_idx & 0x01], F_OK)) {
        remove(file_name[file_idx & 0x01]);
    }

    for (;file_idx > 0;file_idx--) {
        snprintf(file_name[(file_idx -1) & 0x01], sizeof(file_name[0]), "%s.%d.gz", log_file->file_name, file_idx - 1);
        if (0 == access(file_name[(file_idx -1) & 0x01], F_OK)) {
            rename(file_name[(file_idx - 1) & 0x01], file_name[file_idx & 0x01]);
        }
    }

    snprintf(file_name[0], sizeof(file_name[0]), "%s.0", log_file->file_name);

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "gzip %s", file_name[0]);
    system(cmd);

    sem_post(&log_file->sem);

    return NULL;
}

void _update_log_file(stLogFile_t *log_file)
{
    char file_new_name[256];

    sem_wait(&log_file->sem);

    snprintf(file_new_name, sizeof(file_new_name),"%s.0", log_file->file_name);
    rename(log_file->file_name, file_new_name);

    pthread_t pid=0;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_create(&pid, &attr, _log_iterate, (void *)log_file);
}

void log_file_init(stLogFile_t *log_file, const char *file_name, size_t max_file_size)
{
    sem_init(&log_file->sem, 0, 1);
    strncpy(log_file->file_name, file_name, sizeof(log_file->file_name));
    log_file->max_file_size = max_file_size;
}

size_t log_file_edit_file_size(stLogFile_t *log_file, size_t max_file_size)
{
    if (max_file_size < LOG_FILE_SIZE_MIN)
    {
        max_file_size = LOG_FILE_SIZE_MIN;
    }
    log_file->max_file_size = max_file_size;
    return max_file_size;
}

size_t log_file_set_cache(stLogFile_t *log_file, char *ring_str, size_t item_size, int fix_len)
{
    log_file->queue_str = ring_str;
    log_file->item = item_size;
    log_file->fix_length = fix_len;

    return item_size * fix_len;
}

size_t log_file_cache2save(stLogFile_t *log_file)
{
    size_t file_size = 0, szMsgLen = 0, idx = 0;
    char *msg_ptr = NULL;
    FILE *fd = NULL;

    fd = fopen(log_file->file_name, "a+");
	if (NULL == fd) {
        fprintf(stderr, "can't open %s\n",log_file->file_name);
		return 0;
	}

    for (idx = 0;idx < log_file->item;idx++)
    {
        msg_ptr = log_file->queue_str + idx * log_file->fix_length;
        szMsgLen = strlen(msg_ptr);
        fwrite(msg_ptr, szMsgLen, 1, fd);
        log_file->cur_file_size += szMsgLen;
    }

	fclose(fd);

    szMsgLen = log_file->cur_file_size;
    if (log_file->cur_file_size >= log_file->max_file_size) 
    {
        _update_log_file(log_file);
        log_file->cur_file_size = 0;
    }

	return szMsgLen;
}


