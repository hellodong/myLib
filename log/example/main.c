

#include "log.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char buff[256];
    char buff2[256];
    char buff3[256];
    int szLen = 0;
    int idx = 0, jdx = 0;

    for (jdx = 0; jdx < (sizeof(buff));jdx++)
    {
        buff[jdx] = '*';
        buff2[jdx] = '+';
        buff3[jdx] = '#';
    }

    char valStr[12];

    log_system("=====================================");
    loggernofmt(1, LOG_LEVEL_SYSTEM,__FUNCTION__,"++++++++++++++++++++++++++++++++++++++");
    loggernofmt(2, LOG_LEVEL_SYSTEM,__FUNCTION__,"######################################");

    for (jdx = 0;jdx < 80;jdx++)
    {
        int tmp =(jdx + 1)  << 12;
        for (idx = jdx << 12; idx < tmp;idx++)
        {
            szLen = sprintf(valStr, "%d", idx);

            snprintf(buff + 254 - szLen , szLen + 1, "%d", idx);
            snprintf(buff2 + 254 - szLen , szLen + 1, "%d", idx);
            snprintf(buff3 + 254 - szLen , szLen + 1, "%d", idx);
            loggernofmt(0, LOG_LEVEL_WARN,__FUNCTION__,buff);
            loggernofmt(1, LOG_LEVEL_WARN,__FUNCTION__,buff2);
            loggernofmt(2, LOG_LEVEL_WARN,__FUNCTION__,buff3);
        }
        usleep(50000);
    }
    
    log_warn("=====================================");
    loggernofmt(1, LOG_LEVEL_SYSTEM,__FUNCTION__,"++++++++++++++++++++++++++++++++++++++");
    loggernofmt(2, LOG_LEVEL_SYSTEM,__FUNCTION__,"######################################");

    sleep(1);

    return 0;
}
