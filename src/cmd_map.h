#ifndef __CMD_MAP_H__
#define __CMD_MAP_H__

#define SIZE_OF(n)      (sizeof(n)/sizeof(n[0]))

#define gotoxy(x,y)    printf("\033[%d;%dH", (y), (x))

typedef int (*cli_cmd_call_fn)(void *);
typedef struct cmd_entry    cmd_entry_t;

struct cmd_entry {
    char *cmd;
    char *help;
    cli_cmd_call_fn func;
    cmd_entry_t *child;
};

extern cmd_entry_t disable_cmd_map[];

extern void print_help(cmd_entry_t cmd_map[]);

#endif
