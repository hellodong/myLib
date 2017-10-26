#ifndef WRITELOG_H
#define WRITELOG_H


//#define WRITE_LOG
#define PRINT_LOG


void WriteLogFile(const char *pszFileName, const char *pszFunctionName, unsigned int iCodeLine, unsigned int  iLogLevel, const char *pszContent);


#define _FATAL       0     // 严重错误
#define _ERROR       1     // 一般错误
#define _WARN        2     // 警告
#define _INFO        3     // 一般信息
#define _TRACE       4     // 跟踪信息
#define _DEBUG       5     // 调试信息


#define WRITELOG(level, format, arg...)                                             \
        do{                                                                         \
                if (level < _WARN) {                                             \
                char tempStr[4096];                                                 \
                snprintf(tempStr, sizeof(tempStr),format,##arg);                    \
                WriteLogFile(__FILE__, __FUNCTION__, __LINE__, level, tempStr);     \
                }                                                                   \
        }while(0)
	


#define WRITELOG_FATAL(format, arg...)            WRITELOG(_FATAL,format, ##arg) 
#define WRITELOG_ERROR(format, arg...)            WRITELOG(_ERROR, format, ##arg) 
#define WRITELOG_WARN(format, arg...)             WRITELOG(_WARN, format, ##arg)
#define WRITELOG_INFO(format, arg...)             WRITELOG(_INFO, format, ##arg)
#define WRITELOG_TRACE(format, arg...)            WRITELOG(_TRACE, format, ##arg)
#define WRITELOG_DEBUG(format, arg...)            WRITELOG(_DEBUG, format, ##arg)

#define PRINTLOG_FATAL(format,arg...)            fprintf(stderr,"%-5s [%s] %s() #%d- " format,"FATAL", __FILE__,__FUNCTION__,__LINE__,##arg)
#define PRINTLOG_ERROR(format,arg...)            fprintf(stderr,"%-5s [%s] %s() #%d- " format,"ERROR", __FILE__,__FUNCTION__,__LINE__,##arg)
#define PRINTLOG_WARN(format,arg...)             fprintf(stdout,"%-5s [%s] %s() #%d- " format,"WARN", __FILE__,__FUNCTION__,__LINE__,##arg)
#define PRINTLOG_INFO(format,arg...)             fprintf(stdout,"%-5s [%s] %s() #%d- " format,"INFO", __FILE__,__FUNCTION__,__LINE__,##arg)
#define PRINTLOG_TRACE(format,arg...)            fprintf(stdout,"%-5s [%s] %s() #%d- " format,"TRACE", __FILE__,__FUNCTION__,__LINE__,##arg)
#define PRINTLOG_DEBUG(format,arg...)            fprintf(stdout,"%-5s [%s] %s() #%d- " format,"DEBUG", __FILE__,__FUNCTION__,__LINE__,##arg)

#if defined WRITE_LOG
#define LOG_FATAL(format, arg...)                WRITELOG_FATAL(format, ##arg)                  
#elif defined  PRINT_LOG
#define LOG_FATAL(format, arg...)                PRINTLOG_FATAL(format, ##arg) 
#else
#define LOG_FATAL(format, arg...)
#endif


#if defined WRITE_LOG
#define LOG_ERROR(format, arg...)                WRITELOG_ERROR(format, ##arg)
#elif defined  PRINT_LOG
#define LOG_ERROR(format, arg...)                PRINTLOG_ERROR(format, ##arg)
#else
#define LOG_ERROR(format, arg...)                
#endif


#if defined WRITE_LOG
#define LOG_WARN(format, arg...)		WRITELOG_WARN(format, ##arg)	                
#elif defined  PRINT_LOG
#define LOG_WARN(format, arg...)                PRINTLOG_WARN(format, ##arg) 
#else
#define LOG_WARN(format, arg...)                
#endif

#if defined WRITE_LOG
#define LOG_INFO(format, arg...)                WRITELOG_INFO(format, ##arg)
#elif defined  PRINT_LOG
#define LOG_INFO(format, arg...)                PRINTLOG_INFO(format, ##arg) 
#else
#define LOG_INFO(format, arg...)                
#endif


#if defined WRITE_LOG
#define LOG_TRACE(format, arg...)              WRITELOG_TRACE(format, ##arg)
#elif defined  PRINT_LOG
#define LOG_TRACE(format, arg...)              PRINTLOG_TRACE(format, ##arg)  
#else
#define LOG_TRACE(format, arg...)                
#endif


#if defined WRITE_LOG
#define LOG_DEBUG(format, arg...) 	      WRITELOG_DEBUG(format, ##arg)
#elif defined  PRINT_LOG
#define LOG_DEBUG(format, arg...)             PRINTLOG_DEBUG(format, ##arg)   
#else
#define LOG_DEBUG(format, arg...)                
#endif


#endif /* writeLog.h */
