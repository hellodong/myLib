#ifndef _TEST_LOG_H_
#define _TEST_LOG_H_


#include "log_buf.h"
#include "log_file.h"
#include "log.h"

#ifdef __cplusplus
extern "C"{
#endif


stLogFile_t *_log_get_log_file(int _mod);


#ifdef __cplusplus
}
#endif

#endif
