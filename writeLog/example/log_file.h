#ifndef _LOG_FILE_H_
#define _LOG_FILE_H_


enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
};


#define log_debug(format, arg...)       logFileWrite(__FUNCTION__,__LINE__,LOG_LEVEL_DEBUG, format, ##arg)           
#define log_info(format, arg...)         logFileWrite(__FUNCTION__,__LINE__,LOG_LEVEL_INFO, format, ##arg)              
#define log_warn(format, arg...)        logFileWrite(__FUNCTION__,__LINE__,LOG_LEVEL_WARN, format, ##arg)              
#define log_error(format, arg...)        logFileWrite(__FUNCTION__,__LINE__,LOG_LEVEL_ERROR, format, ##arg)              
#define log_fatal(format, arg...)       logFileWrite(__FUNCTION__,__LINE__,LOG_LEVEL_FATAL, format, ##arg)            

int log_file_init(const char *path, int max_file_size, int level, int is_print);

int logFileWrite(const char *fun_name,int line, int level, const char *format, ...);


#endif 
