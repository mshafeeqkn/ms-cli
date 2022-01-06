#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    return ret;
}

void ms_cmd_free(ms_cmd_t *cmd) {
    free(cmd->str);
    free(cmd->prefix);
    free(cmd);
}

void ms_cmd_print(ms_cmd_t *cmd) {
    ms_log("%s %s\tlen: %d, pos: %d",
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
    ms_cmd_print(cmd);
}
