

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


char g_logLevel[][128]={"FATAL",
						"ERROR",
						"WARN",
						"INFO",
						"TRACE",
						"DEBUG"};

static void getData(char *str, size_t len);
static void getTime(char*pszTimeStr, size_t len);


int WriteLogFile(const char *pszFileName, const char *pszFunctionName, unsigned int iCodeLine, unsigned int  iLogLevel, const char *pszContent)
 {
    FILE  *fp                = NULL;
    char  szTimeStr[128]     = {0};
    char  logName[64];
    char  dataStr[32];
    size_t length = 0;

    if (pszFileName == NULL || pszContent == NULL)
    {
        return -1;
    }

    memset(logName, 0x00, sizeof(logName));
    memset(dataStr, 0x00, sizeof(dataStr));
    getData(dataStr, sizeof(dataStr));
    snprintf(logName,sizeof(logName),"%s.txt",dataStr);
    fp = fopen(logName, "at+");      				// 打开文件, 每次写入的时候在后面追加
    if (fp == NULL)
    {
        return;
    }

    // 写入日志时间
    getTime(szTimeStr, sizeof(szTimeStr));

    // 写入日志内容
    length = fprintf(fp, "%-5s [%s][%s] %s() #%d: %s\r\n", g_logLevel[iLogLevel], szTimeStr, pszFileName, pszFunctionName, iCodeLine, pszContent);
   
    fflush(fp);     // 刷新文件
    fclose(fp);     // 关闭文件
    fp = NULL;      // 将文件指针置为空

    return length;
}

void getData(char *str, size_t len)
{
    if (NULL == str || 0 == len)
    {
        return;
    }
    struct tm sysData;
    time_t curTime;

    curTime = time(NULL);
    localtime_r(&curTime, &sysData);
    snprintf(str, len,"%04d%02d%02d",sysData.tm_year + 1900, sysData.tm_mon + 1,sysData.tm_mday);

    return;
}

void getTime(char*pszTimeStr, size_t len)
{
    struct tm      tSysTime     = {0};
    struct timeval tTimeVal     = {0};
    time_t         tCurrentTime = {0};

    char  szMsec[20] = {0};    // 毫秒

    if (NULL == pszTimeStr || len < 1)
    {
        return;
    }

    tCurrentTime = time(NULL);
    localtime_r(&tCurrentTime, &tSysTime);   // localtime_r是线程安全的

    gettimeofday(&tTimeVal, NULL);    
    snprintf(szMsec, sizeof(szMsec),"%03ld", tTimeVal.tv_usec / 1000);

    snprintf(pszTimeStr, len, "%02d:%02d:%02d.%3.3s", tSysTime.tm_hour, tSysTime.tm_min, tSysTime.tm_sec, szMsec);
    return;
}