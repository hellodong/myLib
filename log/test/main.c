

#include "log.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char buff[256];
    int szLen = 0;
    int idx = 0, jdx = 0;

    for (jdx = 0; jdx < (sizeof(buff));jdx++)
    {
        buff[jdx] = '*';
    }

    char valStr[12];

    for (jdx = 0;jdx < 16;jdx++)
    {
   
        int tmp =(jdx + 1)  << 12;
        for (idx = jdx << 12; idx < tmp;idx++)
        {
            szLen = sprintf(valStr, "%d", idx);

            snprintf(buff + 254 - szLen , szLen + 1, "%d", idx);
            loggernofmt(0, LOG_LEVEL_WARN,__FUNCTION__,buff);
        }
        sleep(1);
    }

    sleep(2);

    return 0;
}
