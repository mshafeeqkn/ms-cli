#ifndef __MS_LOG_H__
#define __MS_LOG_H__

#include <string.h>

#define ms_log(level__, fmt__, args__...) \
    {_ms_log(level__, strrchr(__FILE__, '/') + 1, __LINE__, fmt__, ##args__);}

#define ms_err(fmt__, args__...) ms_log(log_err, fmt__, ##args__)
#define ms_info(fmt__, args__...) ms_log(log_info, fmt__, ##args__)
#define ms_dbg(fmt__, args__...) ms_log(log_dbg, fmt__, ##args__)
#define ms_mem(fmt__, args__...) ms_log(log_mem, fmt__, ##args__)

typedef enum {
    log_err,
    log_info,
    log_dbg,
    log_mem,
} log_level_t;

extern void _ms_log(log_level_t level, char *file, int line,  const char* format, ...);

extern void ms_log_load_log_file();

#endif
