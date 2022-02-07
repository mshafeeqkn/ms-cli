#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#include "ms_entry.h"
#include "ms_log.h"
#include "ms_cmd.h"
#include "ms_mem.h"


void configure_stdin() {
    struct termios info;

    tcgetattr(0, &info);                /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    info.c_lflag &= ~(ICANON|ECHO);     /* disable canonical mode and echo */
    info.c_cc[VMIN] = 1;                /* wait until at least one keystroke available */
    info.c_cc[VTIME] = 0;               /* no timeout */
    tcsetattr(0, TCSANOW, &info);       /* set immediately */
}

void ms_complete_command(ms_cmd_t *cmd_tree, ms_entry_t *entry) {
   ms_cmd_t *node = ms_cmd_get_matching_cmd(cmd_tree, entry);
   node = node;
   // ms_entry_set_string(entry, node->cmd);
}

int main(int argc, char *argv[]) {
    char ch;
    ms_entry_t *entry, *tmp;
    ms_entry_t *head;
    ms_cmd_t *cmd_tree = NULL;
    ms_cmd_t *cmd_head;

    ms_log_load_log_file();
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
                ms_complete_command(cmd_tree, entry);
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
                cmd_head = cmd_tree;
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
        ms_cmd_update_cmd_head(cmd_tree, entry, &cmd_head);
    }

    ms_entry_free(entry);
    return 0;
}
