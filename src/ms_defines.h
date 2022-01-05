#ifndef __MS_DEFINES_H__
#define __MS_DEFINES_H__

typedef enum {
    cli_disabled = 0,
    cli_enabled,
    cli_configure,
} cli_state_t;

typedef enum {
    ms_status_ok,
    ms_mem_fail,
    ms_arg_null,
    ms_status_unknown
} ms_status_t;

#endif
