

#include "logFile.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>


typedef struct
{
	char path[128];
	int nowLine;
	int maxLine;
}LogFileCtx_t;


static LogFileCtx_t g_ctx;

static int findMaxLine(FILE *fd, long *offsetPtr)
{
	char temp[1024];
	char ch[]=" ";
	char *rp = NULL;
	int maxLine = -1, line;

	memset(temp, 0x00, sizeof(temp));
	while(fgets(temp, sizeof(temp), fd)){
		rp = strtok(temp, ch);
		line = atoi(rp);
		memset(temp, 0x00, sizeof(temp));
		if (line > maxLine) {
			maxLine = line;
			*offsetPtr = ftell(fd);
		}
	}
	return maxLine;

}


static int findNowLine(FILE *fd)
{
	long offset, maxLine;

	maxLine = findMaxLine(fd, &offset);

	return maxLine;
}

int logFileInit(const char *path, int maxLine)
{
	strcpy(g_ctx.path, path);
	g_ctx.maxLine = maxLine;
	if (!access(path, F_OK)){
		FILE *fileFd = NULL;
		int nowLine;

		fileFd = fopen(g_ctx.path, "a+");
		if (NULL == fileFd) {
			return -1;
		}

		fclose(fileFd);
	}else{
		g_ctx.nowLine = 0;
	}
	return 0;
}

int logFileRecd(const char data[])
{
	if (NULL == g_ctx.path || 0 == g_ctx.maxLine) {
		return -1;
	}
	FILE *fileFd = NULL;

	fileFd = fopen(g_ctx.path, "a+");
	if (NULL == fileFd) {
		return 0;
	}

	fclose(fileFd);
	return strlen(data);
}
