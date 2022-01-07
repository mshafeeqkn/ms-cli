#include <stdio.h>
#include <stdlib.h>

#include "ms_console.h"
#include "ms_defines.h"
#include "ms_log.h"

extern ms_cmd_t* ms_cmd_create(char *str) {
    int len = strlen(str);

    ms_cmd_t *ret = malloc(sizeof(ms_cmd_t));
    if(!ret)
        return NULL;

    ret->str = malloc(len + 1);
    if(!ret->str) {
        free(ret);
        return NULL;
    }

    strncpy(ret->str, str, len);
    ret->str[len] = 0;

    ret->len = len;
    ret->curser = len;
    ret->next = NULL;
    ret->prev = NULL;

    ms_log(log_dbg, "Created new cmd node: 0x%x", addr(ret));
    return ret;
}

void ms_cmd_free(ms_cmd_t *cmd) {
    free(cmd->str);
    free(cmd->prefix);
    free(cmd);
}

void _ms_cmd_print(char *pref, ms_cmd_t *cmd) {
    ms_log(log_dbg, "%s - 0x%06x<(this:0x%06x)>0x%06x %s %-10s\tlen: %d, pos: %d",
        pref, addr(cmd->prev), addr(cmd), addr(cmd->next),
        cmd->prefix, cmd->str, cmd->len, cmd->curser);
}

ms_status_t ms_cmd_set_prefix(ms_cmd_t *cmd, char *prefix) {
    int pref_len = strlen(prefix);

    if(!cmd || !prefix)
        return -ms_st_null_arg;

    cmd->prefix = malloc(pref_len + 1);
    if(!cmd->prefix)
        return -ms_st_mem_err;

    strncpy(cmd->prefix, prefix, pref_len);
    cmd->prefix[pref_len] = 0;
    return ms_st_ok;
}

ms_status_t ms_cmd_set_curser(ms_cmd_t *cmd, int curser) {
    if(!cmd)
        return -ms_st_null_arg;

    if(curser < -1 || curser > cmd->len)
        return -ms_st_inval_arg;

    cmd->curser = curser;

    return ms_st_ok;
}

ms_status_t ms_cmd_curser_fw(ms_cmd_t *cmd) {
    if(cmd->curser == cmd->len)
        return -ms_st_limit_exhaust;

    cmd->curser++;
    return ms_st_ok;
}

ms_status_t ms_cmd_curser_bw(ms_cmd_t *cmd) {
    if(cmd->curser == 0)
        return -ms_st_limit_exhaust;

    cmd->curser--;
    return ms_st_ok;
}

ms_status_t ms_cmd_insert_char_at(ms_cmd_t *cmd, int pos, char ch) {
    char tmp[MAX_COMMAND_LEN] = {0};
    ms_status_t ret;
    int char_index;

    if(pos < 0 || pos > cmd->len)
        return -ms_st_inval_arg;

    strncpy(tmp, cmd->str, pos);

    // char_index is the index at which the new char 'ch'
    // has to be inserted. 
    tmp[pos] = ch;
    strcpy(tmp + pos + 1, cmd->str + pos);

    cmd->len++;
    ret = ms_cmd_set_curser(cmd, pos + 1);
    if(ret != ms_st_ok)
        return ret;

    free(cmd->str);
    cmd->str = malloc(cmd->len);
    if(!cmd->str)
        return -ms_st_mem_err;

    strncpy(cmd->str, tmp, cmd->len);
    cmd->str[cmd->len] = 0;

    return ms_st_ok;
}

ms_status_t ms_cmd_insert_char(ms_cmd_t *cmd, char ch) {
    return ms_cmd_insert_char_at(cmd, cmd->curser, ch);
}

ms_status_t ms_cmd_remove_char(ms_cmd_t *cmd) {
    if(cmd->curser == 0 || cmd->curser > cmd->len)
        return -ms_st_fail;

    if(cmd->len != cmd->curser) {
        strcpy(cmd->str + cmd->curser - 1, cmd->str + cmd->curser);
    }

    cmd->curser--;
    cmd->len--;
    cmd->str[cmd->len] = 0;
    return ms_st_ok;
}

void ms_print_console(ms_cmd_t *cmd) {
    char buff[MAX_COMMAND_LEN] = {0};
    snprintf(buff, MAX_COMMAND_LEN, "%s %s", cmd->prefix, cmd->str);
    printf("\033[2K");
    printf("\r%s", buff);
    if(cmd->len != cmd->curser)
        printf("\033[%dD", cmd->len - cmd->curser);
    fflush(stdout);
}

ms_status_t ms_cmd_hook_after(ms_cmd_t *head, ms_cmd_t *cmd) {
    if(head == NULL || cmd == NULL) {
        return -ms_st_null_arg;
    }

    if(cmd->next) {
        cmd->next->prev = cmd->prev;
        cmd->prev->next = cmd->next;
    }
    while(head->next)
        head = head->next;

    head->next = cmd;
    cmd->prev = head;

    return ms_st_ok;
}


ms_status_t ms_cmd_copy_data(ms_cmd_t *dst, ms_cmd_t *src) {
    int pref_len = strlen(src->prefix);

    if(dst->str)
        free(dst->str);
    dst->str = calloc(src->len + 1, sizeof(char));

    if(dst->prefix)
        free(dst->prefix);
    dst->prefix = calloc(pref_len + 1, sizeof(char));

    strncpy(dst->str, src->str, src->len);
    strncpy(dst->prefix, src->prefix, pref_len);
    dst->len = src->len;
    dst->curser = src->len;
}

ms_status_t ms_cmd_copy_to_list_end(ms_cmd_t *head, ms_cmd_t *node) {
    ms_cmd_t *tmp = ms_cmd_create(node->str);
    ms_cmd_set_prefix(tmp, node->prefix);
    memset(tmp, 0, sizeof(ms_cmd_t));

    ms_cmd_t *tail = head;
    while(tail->next)
        tail = tail->next;

    if(tail->len == 0)
        tail = tail->prev;
        ms_cmd_free(tail->next);

    tail->next = tmp;
    tmp->prev = tail;
    ms_cmd_copy_data(tmp, node);
}


ms_status_t ms_command_print_history(ms_cmd_t *cmd, int fw) {
    while(cmd) {
         ms_cmd_print(cmd);
        if(fw)
            cmd = cmd->next;
        else
            cmd = cmd->prev;
    }
}
