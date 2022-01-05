#ifndef __MS_DEFINES_H__
#define __MS_DEFINES_H__

typedef enum {
    cli_disabled = 0,
    cli_enabled,
    cli_configure,
} cli_state_t;

typedef enum {
    ms_st_ok,
    ms_st_fail,
    ms_st_null_arg,
    ms_st_inval_arg,
    ms_st_limit_exhaust,
    ms_st_unknown
} ms_status_t;

#endif