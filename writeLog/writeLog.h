#ifndef WRITELOG_H
#define WRITELOG_H


#define WRITE_LOG


void WriteLogFile(const char *pszFileName, const char *pszFunctionName, unsigned int iCodeLine, unsigned int  iLogLevel, const char *pszContent);


#define LOG_FATAL       0     // 严重错误
#define LOG_ERROR       1     // 一般错误
#define LOG_WARN        2     // 警告
#define LOG_INFO        3     // 一般信息
#define LOG_TRACE       4     // 跟踪信息
#define LOG_DEBUG       5     // 调试信息


#define WRITELOG(level, format, arg...)                                             \
        do{                                                                         \
                if (level < LOG_WARN) {                                             \
                char tempStr[4096];                                                 \
                snprintf(tempStr, sizeof(tempStr),format,##arg);                    \
                WriteLogFile(__FILE__, __FUNCTION__, __LINE__, level, tempStr);     \
                }                                                                   \
        }while(0)
	


#define WRITELOG_FATAL(format, arg...)            WRITELOG(LOG_FATAL,format, ##arg) 
#define WRITELOG_ERROR(format, arg...)            WRITELOG(LOG_ERROR, format, ##arg) 
#define WRITELOG_WARN(format, arg...)             WRITELOG(LOG_WARN, format, ##arg)
#define WRITELOG_INFO(format, arg...)             WRITELOG(LOG_INFO, format, ##arg)
#define WRITELOG_TRACE(format, arg...)            WRITELOG(LOG_TRACE, format, ##arg)
#define WRITELOG_DEBUG(format, arg...)            WRITELOG(LOG_DEBUG, format, ##arg)





#endif /* writeLog.h */
