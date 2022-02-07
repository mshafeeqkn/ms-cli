#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "ms_cmd.h"
#include "ms_log.h"
#include "ms_mem.h"
#include "ms_utils.h"


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

    cmd = ms_alloc(sizeof(ms_cmd_t));
    if(cmd == NULL)
        return NULL;

    memset(cmd, 0, sizeof(ms_cmd_t));
    cmd->cmd_id = cmd_id;

    cmd->cmd = ms_alloc(cmd_len + 2);
    if(cmd->cmd == NULL)
        goto err_cmd;
    snprintf(cmd->cmd, cmd_len+2, "%s ", cmd_str);

    cmd->help = ms_alloc(help_len + 1);
    if(cmd->help == NULL)
        goto err_help;
    strncpy(cmd->help, help, help_len);
    cmd->help[help_len] = 0;

    cmd->arg_range = ms_alloc(arg_range_len + 1);
    if(cmd->arg_range == NULL)
        goto err_range;
    strncpy(cmd->arg_range, arg_range, arg_range_len);
    cmd->arg_range[help_len] = 0;

    cmd->arg_help_str = ms_alloc(arg_help_len + 1);
    if(cmd->arg_help_str == NULL)
        goto err_arg_help;
    strncpy(cmd->arg_help_str, arg_help, arg_help_len);
    cmd->arg_help_str[arg_help_len] = 0;

    cmd->cmd_cb = cb;
    return cmd;

err_arg_help:
    ms_free(arg_range);
err_range:
    ms_free(cmd->help);
err_help:
    ms_free(cmd->cmd);
err_cmd:
    ms_free(cmd);
    return NULL;
}

void ms_cmd_destroy(ms_cmd_t *cmd) {
    ms_free(cmd->cmd);
    ms_free(cmd->help);
    ms_free(cmd->arg_range);
    ms_free(cmd->arg_help_str);
    if(cmd->arg) ms_free(cmd->arg);
    ms_free(cmd);
}

ms_status_t ms_cmd_show_cmd_help(ms_cmd_t *head, ms_entry_t *entry) {
    char *cmd;
    int len;

    if(head == NULL && entry != NULL) {
        printf("\n<cr>\n");
        return ms_st_ok;
    }

    if(ms_entry_get_last_command(entry, &cmd, &len) != ms_st_ok) {
        return -ms_st_fail;
    }

    while(head) {
        if(len == 0 || strncmp(head->cmd, cmd, len) == 0)
            printf("\n%-20s%s", head->cmd, head->help);
        head = head->next;
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

#define     register_command(id, cmd, help)     { \
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
      register_command(0x03, "mac", "Show MAC infromation");
      register_command(0x04, "mac-addr-table", "Show MAC infromation");
      sub_command_end();
    register_command(0x04, "connect", "Open terminal connection");
    register_command(0x05, "en-test", "Open terminal connection");

    ms_log(log_dbg, "----------Command Register completed---------");
}

ms_cmd_t* ms_cmd_get_cmd_from_str(ms_cmd_t *head, char *cmd, int len) {
    char tmp[128] = {0};

    sprintf(tmp, "%s ", cmd);
    while(head) {
        if(strncmp(head->cmd, tmp, len) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

int ms_cmd_all_matching_cmds(ms_cmd_t *cmd_head, char *token, char ***commands) {
    int token_len = strlen(token);
    char *cmds[256] = {0};
    int i = 0, num;

    ms_dbg("Checking the matches for [%s] in %x", token, addr(cmd_head));

    while(cmd_head) {
        if(strncmp(cmd_head->cmd, token, token_len) == 0) {
            cmds[i++] = cmd_head->cmd;
        }
        cmd_head = cmd_head->next;
    }
    ms_dbg("Total number of matches: %d", i);
    num = i;
    *commands = (char**)malloc(i * sizeof(char*));
    for(;i >= 0; i--) {
        (*commands)[i] = cmds[i];
    }

    return num;
}

void ms_cmd_update_cmd_head(ms_cmd_t *cmd_tree, ms_entry_t *entry, ms_cmd_t **cmd_head) {
    ms_cmd_t *tmp;

    *cmd_head = cmd_tree;

    FOR_EACH_STR(entry->str, tok, len) {
        tmp = ms_cmd_get_cmd_from_str(*cmd_head, tok, len);
        if(tmp != NULL) {
            *cmd_head = tmp->children;
        }
    }
}

static void find_common_prefix(char *cur_prefix, char *cmd) {
    int i = 0;
    if(cur_prefix[0] == 0) {
        strcpy(cur_prefix, cmd);
        return;
    }

    while(cur_prefix[i] && cmd[i] && cur_prefix[i] == cmd[i]) {
        i++;
    }

    cur_prefix[i] = 0;
}

ms_cmd_t* ms_cmd_get_matching_cmd(ms_cmd_t *cmd_tree, ms_entry_t *entry) {
    int matches = 0;
    ms_cmd_t *node = cmd_tree;
    ms_cmd_t *tmp = NULL;
    char match_cmds[1024];
    char match_prefix[1024];

    FOR_EACH_STR(entry->str, part, size) {
        tmp = NULL;
        matches = 0;
        memset(match_cmds, 0, sizeof(match_cmds));
        memset(match_prefix, 0, sizeof(match_prefix));

        while(node) {
            node = ms_cmd_get_cmd_from_str(node, part, size);
            if(!node)
                break;

            if(!tmp)
                tmp = node;

            find_common_prefix(match_prefix, node->cmd);
            matches++;
            strcat(match_cmds, node->cmd);
            strcat(match_cmds, "\t");
            node = node->next;
        }

        if(tmp && tmp->children)
            node = tmp->children;
    }

    ms_dbg("Entry: %s: replace with: %s", entry->str, match_prefix);
    ms_dbg_print_entry(entry);
    ms_entry_replace_last_word(entry, match_prefix);
    ms_dbg_print_entry(entry);

    return node;
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
