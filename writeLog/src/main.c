

#include <stdio.h>

#include "writeLog.h"


int main (void)
{
    LOG_FATAL("this is a test.\r\n");
    LOG_ERROR("this is a test.\r\n");
    LOG_WARN("this is a test.\r\n");
    LOG_INFO("this is a test.\r\n");
    LOG_TRACE("this is a test.\r\n");
    LOG_DEBUG("this is a test.\r\n");
    return 0;
}
