#include <stddef.h>
#include <stdlib.h>

#include "ms_cmd.h"
#include "ms_log.h"


ms_cmd_t* ms_cmd_create(unsigned int cmd_id,
                        char *cmd_str,
                        char *help,
                        char *arg_range,
                        char *arg_help,
                        ms_cmd_cb cb)
{
    ms_cmd_t *cmd;
    int cmd_len = strlen(cmd_str);
    int help_len = strlen(help);
    int arg_range_len = strlen(arg_range);
    int arg_help_len = strlen(arg_help);

    cmd = calloc(1, sizeof(ms_cmd_t));
    if(cmd == NULL)
        return NULL;

    cmd->cmd_id = cmd_id;

    cmd->cmd = malloc(cmd_len + 1);
    if(cmd->cmd == NULL)
        goto err_cmd;
    strncpy(cmd->cmd, cmd_str, cmd_len);
    cmd->cmd[cmd_len] = 0;

    cmd->help = malloc(help_len + 1);
    if(cmd->help == NULL)
        goto err_help;
    strncpy(cmd->help, help, help_len);
    cmd->help[help_len] = 0;

    cmd->arg_range = malloc(arg_range_len + 1);
    if(cmd->arg_range == NULL)
        goto err_range;
    strncpy(cmd->arg_range, arg_range, arg_range_len);
    cmd->arg_range[help_len] = 0;

    cmd->arg_help_str = malloc(arg_help_len + 1);
    if(cmd->arg_help_str == NULL)
        goto err_arg_help;
    strncpy(cmd->arg_help_str, arg_help, arg_help_len);
    cmd->arg_help_str[arg_help_len] = 0;

    cmd->cmd_cb = cb;
    return cmd;

err_arg_help:
    free(arg_range);
err_range:
    free(cmd->help);
err_help:
    free(cmd->cmd);
err_cmd:
    free(cmd);
    return NULL;
}

void ms_cmd_destroy(ms_cmd_t *cmd) {
    free(cmd->cmd);
    free(cmd->help);
    free(cmd->arg_range);
    free(cmd->arg_help_str);
    if(cmd->arg) free(cmd->arg);
    free(cmd);
}

ms_status_t ms_cmd_hook_as_child(ms_cmd_t *parent, ms_cmd_t *child) {
    ms_status_t ret;

    if(parent == NULL || child == NULL)
        return ms_st_null_arg;

    if(parent->children == NULL)
        parent->children = child;
    else {
        ret = ms_cmd_hook_at_end(parent->children, child);
        if(ret != ms_st_ok)
            return ret;
    }
    child->parent = parent;
    return ms_st_ok;
}

ms_status_t ms_cmd_hook_at_end(ms_cmd_t *head, ms_cmd_t *node) {
    if(head == NULL || node == NULL)
        return ms_st_null_arg;

    while(head->next != NULL) {
        head = head->next;
    }

    head->next = node;
    node->prev = head;

    return ms_st_ok;
}

void ms_cmd_dbg_print(ms_cmd_t *cmd) {
    ms_log(log_dbg, "%03x. %s - \t%s", cmd->cmd_id, cmd->cmd, cmd->help);
    ms_log(log_dbg, "\t\t\t%s - %s", cmd->arg_range, cmd->arg_help_str);
    ms_log(log_dbg, "\t\t\tparent: 0x%x; children: 0x%x",
                    addr(cmd->parent), addr(cmd->children));
    ms_log(log_dbg, "\t\t\tprev: 0x%x <this:0x%x> next: 0x%x",
                    addr(cmd->prev), addr(cmd), addr(cmd->next));
}
