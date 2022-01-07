#include <stdio.h>
#include <termios.h>

#include "ms_entry.h"
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
    ms_entry_t *entry, *tmp;
    ms_entry_t *head;

    ms_log(log_dbg, "\033[2J");
    head = entry = ms_entry_create(EMPTY_STR);
    ms_entry_set_prefix(entry, "(Switch)>");
    configure_stdin();

    ms_print_entry(entry);
    while(1) {
        ch = getchar();
        switch(ch) {
            case '?':           // Help
                break;
            case 7:
                ms_log(log_dbg, "======Forward direction=====");
                ms_dbg_print_history(head, 1);
                break;
            case 0x7f:          // Backspace
                ms_entry_remove_char(entry);
                break;
            case '\t':          // Tab
                // if(ms_get_matching_commands(entry)) {
                // }
                putchar('\n');
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
    }

    ms_entry_free(entry);
    return 0;
}
