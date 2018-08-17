

#include "logFile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#define LINE_MAXDATA    (128)


typedef struct
{
	char path[128];
	int nowLine;
	int maxLine;
    long writeOffset;
}LogFileCtx_t;


static LogFileCtx_t g_ctx;
static char g_temp[128];


static int findNowLine(FILE *fd, long *offsetPtr)
{
	char temp[1024];
	char ch[]=" ";
	char *rp = NULL;
	int nowLine = 0, line = 0;

	memset(temp, 0x00, sizeof(temp));
    *offsetPtr = 0;
	while(fgets(temp, sizeof(temp), fd)){
		rp = strtok(temp, ch);
		line = atoi(rp);
		memset(temp, 0x00, sizeof(temp));
		if (line > nowLine) {
			nowLine = line;
			*offsetPtr = ftell(fd);
		}
	}
	return nowLine;
}

static int judgeOffset(long *offsetPtr, int maxLine)
{
    if (*offsetPtr < maxLine * LINE_MAXDATA) {
        return 0;
    }
    *offsetPtr = 0;
    return 1;
}

static char* asciiTime(void)
{
    time_t t;
    struct tm *tmPtr = NULL;
    int len;

    t = time(NULL);
    tmPtr = localtime(&t);
    len = snprintf(g_temp, sizeof(g_temp),"%s", asctime(tmPtr));
    g_temp[len - 1] = ' ';
    return g_temp;
}

static int writeData(FILE *fd, int nowLine,  const char data[])
{
    char temp[LINE_MAXDATA];
    int ret;
    
    memset(temp, ' ', sizeof(temp));
    ret = snprintf(temp, sizeof(temp),"%9d %s %s", nowLine, asciiTime(), data);
    temp[ret] = ' ';
    temp[LINE_MAXDATA - 1] = '\n';
    fwrite(temp, sizeof(temp),1, fd);
    return ret;
}

int logFileInit(const char *path, int maxLine)
{
	strcpy(g_ctx.path, path);
	g_ctx.maxLine = maxLine;
	FILE *fileFd = NULL;

	fileFd = fopen(g_ctx.path, "a+");
	if (NULL == fileFd) {
		return -1;
	}
    g_ctx.nowLine = findNowLine(fileFd, &g_ctx.writeOffset);
	fclose(fileFd);
	return 0;
}

int logFileRecd(const char data[])
{
	if (NULL == g_ctx.path || 0 == g_ctx.maxLine) {
		return -1;
	}
	FILE *fileFd = NULL;
    int ret, retFunc;

	fileFd = fopen(g_ctx.path, "rb+");
	if (NULL == fileFd) {
		return 0;
	}
    rewind(fileFd);
    retFunc = fseek(fileFd, g_ctx.writeOffset, SEEK_SET);
    ret = writeData(fileFd, ++g_ctx.nowLine,  data);
    g_ctx.writeOffset = ftell(fileFd);
    judgeOffset(&g_ctx.writeOffset,g_ctx.maxLine);
	fclose(fileFd);
	return ret;
}

