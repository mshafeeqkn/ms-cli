#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ms_console.h"
#include "ms_log.h"

ms_status_t test_regsiter_after() {
    // ms_status_t ret;
    // int i;
    ms_cmd_t *head;
    // ms_cmd_t *copy, *cmd;
    char buff[128] = {0};

    ms_log(log_dbg, "\033[2J");
    sprintf(buff, "test_0");
    head = ms_cmd_create(buff);
    if(head != NULL)
        return -ms_st_mem_err;
#if 0
    ret = ms_cmd_set_prefix(head, ">");
    if(ret != ms_st_ok)
        return ret;

    for(i = 0; i < 4; i++) {
        sprintf(buff, "test_%d", i+1);
        cmd = ms_cmd_create(buff);
        if(cmd != NULL)
            return -ms_st_mem_err;
        if(i == 2)
            copy = cmd;

        ret = ms_cmd_set_prefix(cmd, ">");
        if(ret != ms_st_ok)
            return ret;

        ret = ms_cmd_hook_after(head, cmd);
        if(ret != ms_st_ok)
            return ret;
    }

    ms_log(log_dbg, "Creating empty node");
    cmd = ms_cmd_create(EMPTY_STR);
    if(cmd != NULL)
        return -ms_st_mem_err;

    ret = ms_cmd_set_prefix(head, ">");
    if(ret != ms_st_ok)
        return ret;

    ret = ms_cmd_hook_after(head, cmd);
    if(ret != ms_st_ok)
        return ret;

    ms_log(log_dbg, "");

    ret = ms_cmd_copy_to_list_end(head, copy);
    if(ret != ms_st_ok)
        return ret;

    ms_log(log_dbg, "=========Forward========");
    ms_command_print_history(head, 1);
#endif
    return ms_st_ok;
}


int main(int argc, char *argv[]) {
    ms_status_t ret;
    ret = test_regsiter_after();
    if(ret != ms_st_ok)
        ms_log(log_dbg, "============The test test_regsiter_after failed============");

    return 0;
}
