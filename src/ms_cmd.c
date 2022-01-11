#include <stdio.h>
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

    cmd->cmd = malloc(cmd_len + 2);
    if(cmd->cmd == NULL)
        goto err_cmd;
    snprintf(cmd->cmd, cmd_len+2, "%s ", cmd_str);

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

ms_status_t ms_cmd_show_cmd_help(ms_cmd_t *tree, ms_entry_t *entry, unsigned char *cmd_path, size_t path_size) {
    int i;
    ms_cmd_t *next_level = tree;
    ms_cmd_t *next_level_head = NULL;

    for(i = 0; i < path_size && cmd_path[i] != 0; i++) {
        while(next_level) {
            if(next_level->cmd_id == cmd_path[i]) {
                next_level = next_level->children;
                break;
            }
            next_level = next_level->next;
        }
    }
    
    next_level_head = next_level;
    while(next_level_head->prev)
        next_level_head = next_level_head->prev;

    while(next_level_head) {
        if(entry->len == 0 || strncmp(next_level_head->cmd, entry->str, entry->len) == 0)
            printf("\n%-10s\t\t%s", next_level_head->cmd, next_level_head->help);
        next_level_head = next_level_head->next;
    }
    putchar('\n');
    return ms_st_ok;
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

#define     register_command(id, cmd, help)         { \
                                                    ms_log(log_dbg, "Adding command: %s", cmd); \
                                                    tmp = ms_cmd_create(id, cmd, help, "1-24", "Help arg", NULL); \
                                                    if(cur_cmd == NULL) {\
                                                        *command_tree = cur_cmd = tmp; \
                                                    } else { \
                                                        if(sub_command == 0) { \
                                                            ms_cmd_hook_at_end(cur_cmd, tmp); \
                                                            tmp->parent = cur_cmd->parent; \
                                                            cur_cmd = cur_cmd->next; \
                                                        } else { \
                                                            ms_cmd_hook_as_child(cur_cmd, tmp); \
                                                            cur_cmd = cur_cmd->children; \
                                                            sub_command = 0; \
                                                        } \
                                                    } \
                                                    ms_cmd_dbg_print(tmp); \
                                                }

#define     sub_command_start()                 { sub_command = 1; }

#define     sub_command_end()                   { cur_cmd = cur_cmd->parent; }

void ms_load_commands(ms_cmd_t **command_tree) {
    ms_cmd_t *cur_cmd = NULL;
    ms_cmd_t *tmp = NULL;
    int sub_command = 0;

    register_command(0x01, "enable", "Turn on privileged commands");
    register_command(0x02, "disable", "Turn off privileged commands");
    register_command(0x03, "show", "Show system running configuration");
      sub_command_start();
      register_command(0x01, "ip", "Show IP infromations");
      register_command(0x02, "vlan", "Show VLAN infromations");
        sub_command_start();
        register_command(0x01, "brief", "Show VLAN infromations in brief");
        register_command(0x02, "id", "Show VLAN infromation by VLAN ID");
        register_command(0x03, "name", "Show VLAN infromation by name");
        sub_command_end();
      register_command(0x03, "mac ", "Show MAC infromation");
      sub_command_end();
    register_command(0x04, "connect", "Open terminal connection");
    register_command(0x05, "en-test", "Open terminal connection");

    ms_log(log_dbg, "----------Command Register completed---------");
}

void ms_cmd_dbg_print_tree(ms_cmd_t *tree) {
    while(tree) {
        ms_cmd_dbg_print(tree);
        if(tree->children)
            tree = tree->children;
        if(tree->next == NULL)
            tree = tree->parent;
        tree = tree->next;
    }
}

void ms_cmd_dbg_print(ms_cmd_t *cmd) {
    ms_log(log_dbg, "%03x. %s - \t%s", cmd->cmd_id, cmd->cmd, cmd->help);
    ms_log(log_dbg, "\t\t\t%s - %s", cmd->arg_range, cmd->arg_help_str);
    ms_log(log_dbg, "\t\t\tparent: 0x%x; children: 0x%x",
                    addr(cmd->parent), addr(cmd->children));
    ms_log(log_dbg, "\t\t\tprev: 0x%x <this:0x%x> next: 0x%x",
                    addr(cmd->prev), addr(cmd), addr(cmd->next));
}
