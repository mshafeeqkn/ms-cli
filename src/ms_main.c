#include <stdio.h>
#include <termios.h>

#include "ms_console.h"
#include "ms_log.h"


void configure_stdin() {
    struct termios info;

    tcgetattr(0, &info);                /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    info.c_lflag &= ~(ICANON|ECHO);     /* disable canonical mode and echo */
    info.c_cc[VMIN] = 1;                /* wait until at least one keystroke available */
    info.c_cc[VTIME] = 0;               /* no timeout */
    tcsetattr(0, TCSANOW, &info);       /* set immediately */
}

int main(int argc, char *argv[]) {
    char ch;
    ms_cmd_t *cmd, *tmp;
    ms_cmd_t *head;

    ms_log(log_dbg, "\033[2J");
    head = cmd = ms_cmd_create(EMPTY_STR);
    ms_cmd_set_prefix(cmd, "(Switch)>");
    configure_stdin();

    ms_print_console(cmd);
    while(1) {
        ch = getchar();
        switch(ch) {
            case '?':           // Help
                break;
            case 7:
                ms_log(log_dbg, "======Forward direction=====");
                ms_command_print_history(head, 1);
                break;
            case 0x7f:          // Backspace
                ms_cmd_remove_char(cmd);
                break;
            case '\t':          // Tab
                putchar('\n');
                break;
            case '\n':          // Return
                if(cmd->str[0]) {
                    // create new if command is already entered
                    if(cmd->next) {
                        // The command taken from the history. So there will be
                        // an emtpy node at the end of the list. Copy the content
                        // of node selected from hisotry to the empty node.
                        ms_cmd_copy_to_list_end(head, cmd);
                    }
                    // Some command is already typed and this is the last node.
                    tmp = ms_cmd_create(EMPTY_STR);
                    ms_cmd_set_prefix(tmp, "(Switch)>");
                    ms_cmd_hook_after(head, tmp);
                    cmd = tmp;
                }
                putchar('\n');
                break;
            case 0x1b:
                getchar();      // Discard [
                switch(getchar()) {
                    case 'A':   // Up
                        if(cmd->prev)
                            cmd = cmd->prev;
                        break;
                    case 'B':   // Down
                        if(cmd->next)
                            cmd = cmd->next;
                        break;
                    case 'C':   // Right
                        ms_cmd_curser_fw(cmd);
                        break;
                    case 'D':   // Left
                        ms_cmd_curser_bw(cmd);
                        break;
                }
                break;
            default:
                ms_cmd_insert_char(cmd, ch);
                break;
        }
        ms_print_console(cmd);
    }

    ms_cmd_free(cmd);
    return 0;
}
