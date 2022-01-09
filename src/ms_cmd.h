#ifndef __MS_CMD_H__
#define __MS_CMD_H__

#include "ms_defines.h"

typedef struct ms_cmd 	ms_cmd_t;

typedef void *(*ms_cmd_cb)(void*);

struct ms_cmd {
    unsigned int cmd_id;            // Index of command
    char *cmd;                      // Command
    char *help;                     // Help text
    char *arg;                      // Argument value [Optional]
    char *arg_range;                // Argument range to be disaplyed in the help
    char *arg_help_str;             // Argument help text
    ms_cmd_cb cmd_cb;               // Callback function to be invoked
    ms_cmd_t *children;             // Head of children node
    ms_cmd_t *parent;               // Parent node
    ms_cmd_t *next;                 // Next node
    ms_cmd_t *prev;                 // Previous node
};


extern ms_cmd_t* ms_cmd_create(unsigned int cmd_id,
                               char *cmd,
                               char *help,
                               char *arg_range,
                               char *arg_help,
                               ms_cmd_cb cb);

extern void ms_cmd_destroy(ms_cmd_t *cmd);

ms_status_t ms_cmd_hook_as_child(ms_cmd_t *parent, ms_cmd_t *child);

ms_status_t ms_cmd_hook_at_end(ms_cmd_t *head, ms_cmd_t *node);

extern void ms_cmd_dbg_print(ms_cmd_t *cmd);

#endif /* __MS_CMD_H__ */
