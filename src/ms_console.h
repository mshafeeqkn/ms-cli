#ifndef __MS_CONSOLE_H__
#define __MS_CONSOLE_H__

#include "ms_defines.h"

#define ms_cmd_print(n)     _ms_cmd_print(#n, n)

typedef struct ms_cmd   ms_cmd_t;

struct ms_cmd{
    char *str;
    int len;
    int curser;
    char *prefix;
    ms_cmd_t *next;
    ms_cmd_t *prev;
};

extern ms_cmd_t* ms_cmd_create(char *str);

extern void ms_cmd_free(ms_cmd_t *cmd);

extern void _ms_cmd_print(char *pref, ms_cmd_t *cmd);

extern ms_status_t ms_cmd_set_prefix(ms_cmd_t *cmd, char *prefix);

extern ms_status_t ms_cmd_set_curser(ms_cmd_t *cmd, int curser);

extern ms_status_t ms_cmd_curser_fw(ms_cmd_t *cmd);

extern ms_status_t ms_cmd_curser_bw(ms_cmd_t *cmd);

extern ms_status_t ms_cmd_insert_char_at(ms_cmd_t *cmd, int pos, char ch);

extern ms_status_t ms_cmd_insert_char(ms_cmd_t *cmd, char ch);

extern ms_status_t ms_cmd_remove_char(ms_cmd_t *cmd);

extern void ms_print_console(ms_cmd_t *cmd);

extern ms_status_t ms_cmd_hook_after(ms_cmd_t *parent, ms_cmd_t *cmd);

extern ms_status_t ms_command_print_history(ms_cmd_t *cmd, int fw);

extern ms_status_t ms_cmd_copy_to_list_end(ms_cmd_t *head, ms_cmd_t *node);

extern ms_status_t ms_cmd_copy_data(ms_cmd_t *dest, ms_cmd_t *src);

#endif
