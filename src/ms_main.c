#include <stdio.h>
#include <termios.h>

#include "ms_console.h"


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
    ms_cmd_t *cmd;

    cmd = ms_cmd_create("");
    ms_cmd_set_prefix(cmd, "(Switch)>");
    configure_stdin();

    ms_print_console(cmd);
    while(1) {
        ch = getchar();
        switch(ch) {
            case '?':           // Help
                break;
            case 0x7f:          // Backspace
                ms_cmd_remove_char(cmd);
                break;
            case '\t':          // Tab
            case '\n':          // Return
                putchar('\n');
                break;
            case 0x1b:
                getchar();      // Discard [
                switch(getchar()) {
                    case 'A':   // Up
                    case 'B':   // Down
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
