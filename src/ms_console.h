#ifndef __MS_CONSOLE_H__
#define __MS_CONSOLE_H__

#include "ms_defines.h"

typedef struct {
    char *str;
    int len;
    int curser;
    char *prefix;
} cmd_info_t;

extern cmd_info_t* create_cmd_info(char *str);

extern void cmd_info_free(cmd_info_t *info);

extern ms_status_t cmd_info_set_prefix(cmd_info_t *info, char *prefix);

extern void ms_print_console(cmd_info_t *cmd_info);

#endif
