#include <stdio.h>
#include <unistd.h>

#include "ms_console.h"
#include "ms_log.h"


int main(int argc, char *argv) {
    ms_status_t ret;

    ms_cmd_t *cmd = ms_cmd_create("0123");
    ms_cmd_set_prefix(cmd, "(Switch)>");

    sleep(1);
    ms_log("Set curser at index: 2");
    ms_cmd_set_curser(cmd, 0);
    sleep(1);
    ms_log("remove char 1");
    ms_cmd_remove_char(cmd);
    sleep(1);
    ms_log("print console");
    ms_print_console(cmd);
    sleep(1);

    ms_cmd_free(cmd);
    printf("\n");
    return 0;
}
