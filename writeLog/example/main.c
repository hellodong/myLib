

#include "log_file.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    int idx = 0;
    log_file_init("./test.log", 1, LOG_LEVEL_DEBUG, 0);
    for (idx = 0;idx < 0xfffffff;idx++)
    {
        log_debug("%d", idx);
    }
    return 0;
}
