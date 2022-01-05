#ifndef __MS_CONSOLE_H__
#define __MS_CONSOLE_H__

#include "ms_defines.h"

typedef struct {
    char *str;
    int len;
    int curser;
    char *prefix;
} ms_cmd_t;

extern ms_cmd_t* create_ms_cmd(char *str);

extern void ms_cmd_free(ms_cmd_t *cmd);

extern void ms_cmd_print(ms_cmd_t *cmd);

extern ms_status_t ms_cmd_set_prefix(ms_cmd_t *cmd, char *prefix);

extern ms_status_t ms_cmd_set_curser(ms_cmd_t *cmd, int curser);

extern ms_status_t ms_cmd_curser_fw(ms_cmd_t *cmd);

extern ms_status_t ms_cmd_curser_bw(ms_cmd_t *cmd);

extern ms_status_t ms_cmd_insert_char_at(ms_cmd_t *cmd, int pos, char ch);

extern ms_status_t ms_cmd_insert_char(ms_cmd_t *cmd, char ch);

extern void ms_print_console(ms_cmd_t *cmd);

#endif
