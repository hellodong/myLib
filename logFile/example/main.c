

#include "logFile.h"

#include <stdio.h>
#include <string.h>
#include <time.h>



int test (void)
{
    FILE *fd;

    fd = fopen("./test.txt", "a+");
    if (NULL == fd) {
        fprintf(stderr, "File Open Error\n");
        return 1;
    }
    char temp[256]; 
    memset(temp, ' ', sizeof(temp));
    temp[255] = '\n';
    fprintf(fd, "1234 123213123 313123121");
    fwrite(temp, sizeof(temp),1, fd);
    fwrite(temp, sizeof(temp),1, fd);
    fwrite(temp, sizeof(temp),1, fd);
    fclose(fd);
    return 0;
}


int test2 (void)
{
    time_t t;
    struct tm *tmPtr = NULL;
    char temp[128];
    int len;

    t = time(NULL);
    tmPtr = localtime(&t);
    len = snprintf(temp, sizeof(temp),"%s" ,asctime(tmPtr));
    temp[len - 1] = ' ';
    printf("%s", temp);


    return 0;
}

int main(void)
{
    int i;
    logFileInit("./test.txt", 100);
    for(i = 0;i< 100;i++) {
        logFileRecd("hello");
    }
        logFileRecd("hello");
        logFileRecd("hello");
        logFileRecd("hello");
        logFileRecd("hello");
    return 0;
}
