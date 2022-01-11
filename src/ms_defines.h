#ifndef __MS_DEFINES_H__
#define __MS_DEFINES_H__

#define FALSE               0
#define TRUE                !(FALSE)
#define MAX_PROMPT          128
#define MAX_COMMAND_LEN     1024
#define EMPTY_STR           ""
#define MAX_SUB_COMMANDS    32

#define addr(n)     (0xffffff & (unsigned long)n)


typedef enum {
    cli_disabled = 0,
    cli_enabled,
    cli_configure,
} cli_state_t;

typedef enum {
    ms_st_ok,
    ms_st_fail,
    ms_st_mem_err,
    ms_st_null_arg,
    ms_st_inval_arg,
    ms_st_limit_exhaust,
    ms_st_unknown,
    ms_st_none
} ms_status_t;

#endif
