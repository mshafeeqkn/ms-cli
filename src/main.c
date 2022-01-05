#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_map.h"

#define FALSE               0
#define TRUE                !(FALSE)
#define MAX_PROMPT          128
#define MAX_COMMAND_LEN     256

typedef enum {
    cli_disabled = 0,
    cli_enabled,
    cli_configure,
} cli_state_t;


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
#if 1
    char ch;
    struct termios info;
    cli_state_t cur_state = cli_disabled;
    char prompt[MAX_PROMPT] = {0};
    char command[MAX_COMMAND_LEN] = {0};
    int cli_empty = TRUE;
    int cmd_change;
    char cur_cmd[256] = {0};
    int char_count = 0;
    int curs_pos = 0;

    tcgetattr(0, &info);                /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    info.c_lflag &= ~(ICANON|ECHO);     /* disable canonical mode */
    info.c_cc[VMIN] = 1;                /* wait until at least one keystroke available */
    info.c_cc[VTIME] = 0;               /* no timeout */
    tcsetattr(0, TCSANOW, &info);       /* set immediately */

    while(1) {
        if(cmd_change) {
            get_prompt(cur_state, prompt, SIZE_OF(prompt));
            printf("\r%s%s", prompt, command);
            curs_pos = strlen(prompt) + strlen(command);
        }
        {
            FILE *fp = fopen("/dev/pts/2", "w");
            fprintf(fp, "current curser: %d char_count: %d\n", curs_pos, char_count);
            fclose(fp);
        }

        ch = getchar();
        // printf("You pressed: %x\n", ch);
        // continue;
        cmd_change = TRUE;
        switch(ch) {

            case '?':
                show_next_commands(cur_state, cur_cmd);
                break;

            case '\t':
                putchar('\n');
                break;

            case '\n':
                memset(command, 0, sizeof(command));
                putchar('\n');
                char_count = 0;
                break;

            case 0x7f: // Backspace
                printf("\033[2K");
                if(strlen(command)) {
                    command[strlen(command)-1] = 0;
                    char_count--;
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
                cmd_change = FALSE;
                break;

            default:
                command[char_count++] = ch;
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
