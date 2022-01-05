#include <stdio.h>
#include <unistd.h>

#include "ms_console.h"
#include "ms_log.h"


int main(int argc, char *argv) {
    ms_status_t ret;

    ms_cmd_t *cmd = create_ms_cmd("0123456789");
    ms_cmd_set_prefix(cmd, "(Switch)>");
    ms_cmd_set_curser(cmd, 5);
    ms_print_cmd(cmd);

    int count = 7;
    while(count--) {
        ret = ms_cmd_curser_fw(cmd);
        ms_log("Return value FW: %d", ret);
        ms_print_cmd(cmd);
        sleep(1);
    }

    count = 15;
    while(count--) {
        ret = ms_cmd_curser_bw(cmd);
        ms_log("Return value BW: %d", ret);
        ms_print_cmd(cmd);
        sleep(1);
    }

    count = 15;
    while(count--) {
        ret = ms_cmd_curser_fw(cmd);
        ms_log("Return value BW: %d", ret);
        ms_print_cmd(cmd);
        sleep(1);
    }

    ms_cmd_free(cmd);
    return 0;
}
