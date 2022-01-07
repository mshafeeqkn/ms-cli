#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ms_console.h"
#include "ms_log.h"

void test_regsiter_after() {
    ms_status_t ret;
    int i;
    ms_cmd_t *head, *cmd, *copy;
    char buff[128] = {0};

    ms_log(log_dbg, "\033[2J");
    sprintf(buff, "test_0");
    head = ms_cmd_create(buff);
    ms_cmd_set_prefix(head, ">");

    for(i = 0; i < 4; i++) {
        sprintf(buff, "test_%d", i+1);
        cmd = ms_cmd_create(buff);
        if(i == 2)
            copy = cmd;

        ms_cmd_set_prefix(cmd, ">");
        ms_cmd_hook_after(head, cmd);
    }

    ms_log(log_dbg, "Creating empty node");
    cmd = ms_cmd_create(EMPTY_STR);
    ms_cmd_set_prefix(head, ">");
    ms_cmd_hook_after(head, cmd);
    ms_log(log_dbg, "");

    ms_cmd_copy_to_list_end(head, copy);

    ms_log(log_dbg, "=========Forward========");
    ms_command_print_history(head, 1);
}


int main(int argc, char *argv) {
    test_regsiter_after();
}
