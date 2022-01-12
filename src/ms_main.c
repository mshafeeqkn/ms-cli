#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#include "ms_entry.h"
#include "ms_log.h"
#include "ms_cmd.h"


void configure_stdin() {
    struct termios info;

    tcgetattr(0, &info);                /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    info.c_lflag &= ~(ICANON|ECHO);     /* disable canonical mode and echo */
    info.c_cc[VMIN] = 1;                /* wait until at least one keystroke available */
    info.c_cc[VTIME] = 0;               /* no timeout */
    tcsetattr(0, TCSANOW, &info);       /* set immediately */
}

ms_status_t ms_get_matching_commands(ms_cmd_t *cmd_head, ms_entry_t *entry, char ***commands, int *size) {
    int i = 0;
    char *cmds[MAX_SUB_COMMANDS] = {0};
    char *cmd;
    int len;

    ms_entry_get_last_command(entry, &cmd, &len);
    while(cmd_head) {
        if(strncmp(cmd_head->cmd, cmd, len) == 0) {
            cmds[i++] = cmd_head->cmd;
        }
        cmd_head = cmd_head->next;
    }

    *commands = (char**) malloc(i * sizeof(char*));

    i = 0;
    while(cmds[i] != 0) {
        (*commands)[i] = cmds[i];
        i++;
    }

    *size = i;
    if(i == 0)
        return -ms_st_none;

    return ms_st_ok;
}

int main(int argc, char *argv[]) {
    char ch;
    char **commands;
    int num_cmds;
    ms_entry_t *entry, *tmp;
    ms_entry_t *head;
    ms_cmd_t *cmd_tree = NULL;
    ms_cmd_t *cmd_head;

    ms_log(log_dbg, "\033[2J");
    head = entry = ms_entry_create(EMPTY_STR);
    ms_entry_set_prefix(entry, "(Switch)>");
    ms_load_commands(&cmd_tree);
    cmd_head = cmd_tree;
    configure_stdin();

    ms_print_entry(entry);
    while(1) {
        ch = getchar();
        switch(ch) {
            case '?':           // Help
                putchar(ch);
                ms_cmd_show_cmd_help(cmd_head, entry);
                break;
            case 7:             // Ctrl + G - Debug option
                break;
            case 0x7f:          // Backspace
                ms_entry_remove_char(entry);
                break;
            case '\t':          // Tab
                if(ms_get_matching_commands(cmd_head, entry, &commands, &num_cmds) == ms_st_ok) {
                    if(num_cmds == 1) {
                        ms_entry_set_string(entry, commands[0]);
                    } else {
                        putchar('\n');
                    }
                } else {
                    putchar('\n');
                }
                break;
            case '\n':          // Return
                if(entry->str[0]) {
                    // create new if command is already entered
                    if(entry->next) {
                        // The command taken from the history. So there will be
                        // an emtpy node at the end of the list. Copy the content
                        // of node selected from hisotry to the empty node.
                        ms_entry_copy_to_list_end(head, entry);
                    }
                    // Some command is already typed and this is the last node.
                    tmp = ms_entry_create(EMPTY_STR);
                    ms_entry_set_prefix(tmp, "(Switch)>");
                    ms_entry_hook_after(head, tmp);
                    entry = tmp;
                }
                putchar('\n');
                break;
            case 0x1b:
                getchar();      // Discard [
                switch(getchar()) {
                    case 'A':   // Up
                        if(entry->prev)
                            entry = entry->prev;
                        break;
                    case 'B':   // Down
                        if(entry->next)
                            entry = entry->next;
                        break;
                    case 'C':   // Right
                        ms_entry_cursor_fw(entry);
                        break;
                    case 'D':   // Left
                        ms_entry_cursor_bw(entry);
                        break;
                }
                break;
            default:
                ms_entry_insert_char(entry, ch);
                break;
        }
        ms_print_entry(entry);
        if(entry->str[entry->len - 1] == ' ') {
            ms_cmd_t *tmp = ms_cmd_get_next_level_head(cmd_head, entry);
            if(tmp != NULL) {
                cmd_head = tmp;
            }
        }
    }

    ms_entry_free(entry);
    return 0;
}
