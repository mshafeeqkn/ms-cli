#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_map.h"
#include "ms_log.h"
#include "ms_console.h"
#include "ms_defines.h"

#define FALSE               0
#define TRUE                !(FALSE)
#define MAX_PROMPT          128
#define MAX_COMMAND_LEN     256

static void get_prompt(cli_state_t state, char *buff, size_t buff_len) {
    memset(buff, 0, buff_len);
    switch(state) {
        case cli_disabled:
            strncpy(buff, "> ", 2);
            break;
    }
}

static void show_next_commands(cli_state_t state, char *cur_cmd) {
    char out_buff[256] = {0};

    switch(state) {
        case cli_disabled:
            print_help(disable_cmd_map);
            break;
    }
}

int main(int argc, char *argv[]) {
#if 0
    char ch;
    struct termios info;
    cli_state_t cur_state = cli_disabled;
    char prompt[MAX_PROMPT] = {0};
    char command[MAX_COMMAND_LEN] = {0};
    int cli_empty = TRUE;
    char cur_cmd[256] = {0};
    int char_count = 0;
    int curs_pos = 0;

    tcgetattr(0, &info);                /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    info.c_lflag &= ~(ICANON|ECHO);     /* disable canonical mode */
    info.c_cc[VMIN] = 1;                /* wait until at least one keystroke available */
    info.c_cc[VTIME] = 0;               /* no timeout */
    tcsetattr(0, TCSANOW, &info);       /* set immediately */

    while(1) {
        if(ch != 0x1b) {
            get_prompt(cur_state, prompt, SIZE_OF(prompt));
            if(!curs_pos) {
                curs_pos = strlen(prompt);
            }
            printf("\r%s%s", prompt, command);
        }
        ms_log("current curser: %d char_count: %d\n", curs_pos, char_count);
        ms_log("current command: %s\n", command);
        printf("\033[%ldD", char_count - curs_pos + strlen(prompt));

        ch = getchar();
        switch(ch) {

            case '?':
                show_next_commands(cur_state, cur_cmd);
                break;

            case '\t':
                putchar('\n');
                char_count = 0;
                curs_pos = strlen(prompt);
                break;

            case '\n':
                memset(command, 0, sizeof(command));
                putchar('\n');
                char_count = 0;
                curs_pos = strlen(prompt);
                break;

            case 0x7f: // Backspace
                printf("\033[2K");
                if(strlen(command)) {
                    command[strlen(command)-1] = 0;
                    char_count--;
                    curs_pos--;
                }
                break;

            case 0x1b:          // Escape
                getchar();      // Discard
                switch(getchar()) {
                    case 'A':   // Up
                        break;
                    case 'B':   // Down
                        break;
                    case 'C':   // Right
                        if(char_count + 2 > curs_pos) {
                            printf("\033[C");
                            curs_pos++;
                        }
                        break;
                    case 'D':   // Left
                        if(curs_pos > 2) {
                            printf("\033[D");
                            curs_pos--;
                        }
                        break;
                }
                break;

            default:{
                char tmp[1000] = {0};
                ms_log("Moving > : [%s]\n", command + curs_pos - strlen(prompt));
                strcpy(tmp, command+curs_pos-strlen(prompt));
                ms_log("Moving tmp> : [%s]\n", tmp);
                command[curs_pos-strlen(prompt)] = ch;
                ms_log("after insert command: %s\n", command);
                strcpy(command + curs_pos - strlen(prompt) + 1, tmp);
                ms_log("after insert command: %s\n", command);
                curs_pos++;
                char_count++;
                }
                break;
        }
    }
#else
    int number;

    printf(
        "Enter your number in the box below"
    );
    printf("\033[D");
    printf("\033[D");
    printf("\033[D");
    printf("\033[D");
    printf("\033[D");
    scanf("%d", &number);
    return 0;
#endif
    return 0;
}
