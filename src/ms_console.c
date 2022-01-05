#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ms_console.h"
#include "ms_defines.h"

#define MAX_CONSOLE_LEN         1024


extern ms_cmd_t* create_ms_cmd(char *str) {
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

extern void ms_cmd_free(ms_cmd_t *cmd) {
    free(cmd->str);
    free(cmd->prefix);
    free(cmd);
}

ms_status_t ms_cmd_set_prefix(ms_cmd_t *cmd, char *prefix) {
    int pref_len = strlen(prefix);

    if(!cmd || !prefix)
        return -ms_st_null_arg;

    cmd->prefix = malloc(pref_len + 1);
    if(!cmd->prefix)
        return -ms_st_null_arg;

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

#include "ms_log.h"

ms_status_t ms_cmd_curser_bw(ms_cmd_t *cmd) {
    if(cmd->curser == 0)
        return -ms_st_limit_exhaust;

    cmd->curser--;
    return ms_st_ok;
}

void ms_print_cmd(ms_cmd_t *cmd) {
    char buff[MAX_CONSOLE_LEN] = {0};
    snprintf(buff, MAX_CONSOLE_LEN, "%s %s", cmd->prefix, cmd->str);
    printf("\r%s", buff);
    printf("\033[%dD", cmd->len - cmd->curser);
    fflush(stdout);
}
