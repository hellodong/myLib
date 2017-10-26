

#include <stdio.h>

#include "writeLog.h"


int main (void)
{
    WRITELOG_FATAL("this is a test.\r\n");
    WRITELOG_ERROR("this is a test.\r\n");
    WRITELOG_WARN("this is a test.\r\n");
    WRITELOG_INFO("this is a test.\r\n");
    WRITELOG_TRACE("this is a test.\r\n");
    WRITELOG_DEBUG("this is a test.\r\n");
    return 0;
}