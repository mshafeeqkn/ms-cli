#ifndef __MS_LOG_H__
#define __MS_LOG_H__

#include <string.h>

#define ms_log(level__, fmt__, args__...) \
    {_ms_log(level__, strrchr(__FILE__, '/') + 1, __LINE__, fmt__, ##args__);}

typedef enum {
    log_err,
    log_info,
    log_dbg,
    log_mem,
} log_level_t;

extern void _ms_log(log_level_t level, char *file, int line,  const char* format, ...);

#endif
