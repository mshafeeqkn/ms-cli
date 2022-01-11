#ifndef __MS_ENTRY_H__
#define __MS_ENTRY_H__

#include "ms_defines.h"

#define ms_dbg_print_entry(n)     _ms_dbg_print_entry(#n, n)

typedef struct ms_entry   ms_entry_t;

struct ms_entry{
    char *str;
    int len;
    int cursor;
    char *prefix;
    ms_entry_t *next;
    ms_entry_t *prev;
};

/**
 * Create ms_entry_t structure which hold the data
 * related to a command typed on the screen.
 *
 * @param str: The command to be stored in the struct
 * @return NULL if allocation failed
 *         Valid pointer if succeed.
 */
extern ms_entry_t* ms_entry_create(char *str);

/**
 * Destroy an existing ms_entry_t
 *
 * @param entry: The structure to be destroyed
 */
extern void ms_entry_free(ms_entry_t *entry);

/**
 * Internal function to print a particular ms_entry_t for
 * DEBUGGING purpose
 *
 * Note: it recommended to use macro ms_entry_print instead of this function.
 *
 * @param pref: Prefix(identified to be printed before
 *          actual data
 * @param entry: the entry structure.
 */
extern void _ms_dbg_print_entry(char *pref, ms_entry_t *entry);

/**
 * Set prefix that is to be printed before the command.
 * Normally this will be the prompt in the terminal
 *
 * @param entry: the entry structure.
 * @param prefix: The prefix
 */
extern ms_status_t ms_entry_set_string(ms_entry_t *entry, char *command);


extern ms_status_t ms_entry_set_prefix(ms_entry_t *entry, char *prefix);

/**
 * Set cursor at the index 'cursor' in the command
 *
 * @param entry: the entry structure.
 * @param cursor: Index wherein the cursor is to be set
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_entry_set_cursor(ms_entry_t *entry, int cursor);

/**
 * Move cursor forward in the command
 *
 * @param entry: the entry structure.
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_entry_cursor_fw(ms_entry_t *entry);

/**
 * Move cursor backward in the command
 *
 * @param entry: the entry structure.
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_entry_cursor_bw(ms_entry_t *entry);

/**
 * Insert a char at the position pos in the command
 *
 * @param entry: the entry structure.
 * @param pos: position at which the char is to be inserted.
 * @param ch: char to be inserted
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_entry_insert_char_at(ms_entry_t *entry, int pos, char ch);

/**
 * Insert a char just before the cursor in the command
 *
 * @param entry: the entry structure.
 * @param ch: char to be inserted
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_entry_insert_char(ms_entry_t *entry, char ch);

/**
 * Remove a char just before the cursor in the command
 *
 * @param entry: the entry structure.
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_entry_remove_char(ms_entry_t *entry);

/**
 * Print the entry to the console output.
 *
 * @param entry: the entry structure.
 */
extern void ms_print_entry(ms_entry_t *entry);

/**
 * Hook the ms_entry_t structure at the tail of the linked list.
 * This list will be used to navigate through the previous commands.
 *
 * @param head: The head of the list
 * @param entry: the entry structure.
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_entry_hook_after(ms_entry_t *head, ms_entry_t *entry);

/**
 * Print the list starting from entry in forward/reverse direction
 *
 * @param entry: the entry structure.
 * @param fw: 1 for forward direction, 0 for reverse direction
 */
extern void ms_dbg_print_history(ms_entry_t *entry, int fw);

/**
 * Copy an entry (may be from the middle) to the tail. This is normally
 * used when the user select the command from the history/using up/down arrow
 * to select previous command
 *
 * @param head: The head of the list
 * @param node: The node which is to be copied to the tail
 */
extern ms_status_t ms_entry_copy_to_list_end(ms_entry_t *head, ms_entry_t *node);

/**
 * Copy one ms_entry_t to another. the dst should not be null
 *
 * @param dst: The destination to which the data has to be copied
 * @param src: The source from which data has to be copied
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_entry_copy_data(ms_entry_t *dest, ms_entry_t *src);

#endif /* __MS_CONSOLE_H__ */
