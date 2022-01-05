#include <stdio.h>
#include <unistd.h>

#include "ms_console.h"
#include "ms_log.h"


int main(int argc, char *argv) {
    ms_status_t ret;

    ms_cmd_t *cmd = create_ms_cmd("0123");
    ms_cmd_set_prefix(cmd, "(Switch)>");
    // TODO: Issue, curser at 4, but showing at 3
    ms_cmd_set_curser(cmd, 4);
    ms_print_console(cmd);
    sleep(10);

    ms_cmd_free(cmd);
    printf("\n");
    return 0;
}
