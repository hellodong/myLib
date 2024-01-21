

#include "log_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


static const int g_maxfile_size = 10 * 1024 * 1024;


void _log_iterate(void *origin_file_name)
{
    char file_name[2][128];
    int file_idx = 9;

    snprintf(file_name[file_idx & 0x01], sizeof(file_name[0]),"%s.%d.gz", origin_file_name, file_idx);
    if (0 == access(file_name[file_idx & 0x01], F_OK)) {
        remove(file_name[file_idx & 0x01]);
    }

    for (;file_idx > 0;file_idx--) {
        snprintf(file_name[(file_idx -1) & 0x01], sizeof(file_name[0]), "%s.%d.gz", origin_file_name, file_idx - 1);
        if (0 == access(file_name[(file_idx -1) & 0x01], F_OK)) {
            rename(file_name[(file_idx - 1) & 0x01], file_name[file_idx & 0x01]);
        }
    }

    snprintf(file_name[0], sizeof(file_name[0]), "%s.0", origin_file_name);

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "gzip %s", file_name[0]);
    system(cmd);
}

void _update_log_file(const char *file_name)
{
    char file_new_name[256];
    snprintf(file_new_name, sizeof(file_new_name),"%s.0", file_name);
    rename(file_name, file_new_name);

    pthread_t pid=0;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_create(&pid, &attr, _log_iterate, (void *)file_name);
}

size_t log_file_cache2save(const char *ring_str, size_t item_size, int fix_len, const char *file_name)
{
    size_t file_size = 0, szMsgLen = 0, idx = 0;
    FILE *fd = fopen(file_name, "a+");
	if (NULL == fd) {
        fprintf(stderr, "can't open %s\n",file_name);
		return 0;
	}
    fseek(fd, 0, SEEK_END);
    file_size = ftell(fd);

    for (idx = 0;idx < item_size;idx++)
    {
        szMsgLen = strlen(ring_str + idx * fix_len);
        fwrite(ring_str + idx * fix_len, szMsgLen, 1, fd);
        file_size += szMsgLen;
    }

	fclose(fd);

    if (file_size >= g_maxfile_size) {
        _update_log_file(file_name);
    }

	return file_size;
}

