#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ms_console.h"
#include "ms_defines.h"

#define MAX_CONSOLE_LEN         1024


extern cmd_info_t* create_cmd_info(char *str) {
    int len = strlen(str);

    cmd_info_t *ret = malloc(sizeof(cmd_info_t));
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

extern void cmd_info_free(cmd_info_t *info) {
    free(info->str);
    free(info->prefix);
    free(info);
}

ms_status_t cmd_info_set_prefix(cmd_info_t *info, char *prefix) {
    int pref_len = strlen(prefix);

    if(!info || !prefix)
        return ms_arg_null;

    info->prefix = malloc(pref_len + 1);
    if(!info->prefix)
        return ms_mem_fail;

    strncpy(info->prefix, prefix, pref_len);
    info->prefix[pref_len] = 0;
    return ms_status_ok;
}

void ms_print_console(cmd_info_t *info) {
    char buff[MAX_CONSOLE_LEN] = {0};
    snprintf(buff, MAX_CONSOLE_LEN, "%s %s", info->prefix, info->str);
    printf("%s", buff);
}
