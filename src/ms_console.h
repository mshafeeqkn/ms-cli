#ifndef __MS_CONSOLE_H__
#define __MS_CONSOLE_H__

#include "ms_defines.h"

#define ms_cmd_print(n)     _ms_cmd_print(#n, n)

typedef struct ms_cmd   ms_cmd_t;

struct ms_cmd{
    char *str;
    int len;
    int cursor;
    char *prefix;
    ms_cmd_t *next;
    ms_cmd_t *prev;
};

/**
 * Create ms_cmd_t structure which hold the data
 * related to a command typed on the screen.
 *
 * @param str: The command to be stored in the struct
 * @return NULL if allocation failed
 *         Valid pointer if succeed.
 */
extern ms_cmd_t* ms_cmd_create(char *str);

/**
 * Destroy an existing ms_cmd_t
 *
 * @param cmd: The structure to be destroyed
 */
extern void ms_cmd_free(ms_cmd_t *cmd);

/**
 * Internal function to print a particular ms_cmd_t for
 * DEBUGGING purpose
 *
 * Note: it recommended to use macro ms_cmd_print instead of this function.
 *
 * @param pref: Prefix(identified to be printed before
 *          actual data
 * @param cmd: the command structure.
 */
extern void _ms_cmd_print(char *pref, ms_cmd_t *cmd);

/**
 * Set prefix that is to be printed before the command.
 * Normally this will be the prompt in the terminal
 *
 * @param cmd: the command structure.
 * @param prefix: The prefix
 */
extern ms_status_t ms_cmd_set_prefix(ms_cmd_t *cmd, char *prefix);

/**
 * Set cursor at the index 'cursor' in the command
 *
 * @param cmd: the command structure.
 * @param cursor: Index wherein the cursor is to be set
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_cmd_set_cursor(ms_cmd_t *cmd, int cursor);

/**
 * Move cursor forward in the command
 *
 * @param cmd: the command structure.
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_cmd_cursor_fw(ms_cmd_t *cmd);

/**
 * Move cursor backward in the command
 *
 * @param cmd: the command structure.
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_cmd_cursor_bw(ms_cmd_t *cmd);

/**
 * Insert a char at the position pos in the command
 *
 * @param cmd: the command structure.
 * @param pos: position at which the char is to be inserted.
 * @param ch: char to be inserted
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_cmd_insert_char_at(ms_cmd_t *cmd, int pos, char ch);

/**
 * Insert a char just before the cursor in the command
 *
 * @param cmd: the command structure.
 * @param ch: char to be inserted
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_cmd_insert_char(ms_cmd_t *cmd, char ch);

/**
 * Remove a char just before the cursor in the command
 *
 * @param cmd: the command structure.
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_cmd_remove_char(ms_cmd_t *cmd);

/**
 * Print the command to the console output.
 *
 * @param cmd: the command structure.
 */
extern void ms_print_console(ms_cmd_t *cmd);

/**
 * Hook the ms_cmd_t structure at the tail of the linked list.
 * This list will be used to navigate through the previous commands.
 *
 * @param head: The head of the list
 * @param cmd: the command structure.
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_cmd_hook_after(ms_cmd_t *head, ms_cmd_t *cmd);

/**
 * Print the list starting from cmd in forward/reverse direction
 *
 * @param cmd: the command structure.
 * @param fw: 1 for forward direction, 0 for reverse direction
 */
extern void ms_command_print_history(ms_cmd_t *cmd, int fw);

/**
 * Copy a command (may be from the middle) to the tail. This is normally
 * used when the user select the command from the history/using up/down arrow
 * to select previous command
 *
 * @param head: The head of the list
 * @param node: The node which is to be copied to the tail
 */
extern ms_status_t ms_cmd_copy_to_list_end(ms_cmd_t *head, ms_cmd_t *node);

/**
 * Copy one ms_cmd_t to another. the dst should not be null
 *
 * @param dst: The destination to which the data has to be copied
 * @param src: The source from which data has to be copied
 * @return ms_st_ok on success. Appropriate error code on failure
 */
extern ms_status_t ms_cmd_copy_data(ms_cmd_t *dest, ms_cmd_t *src);

#endif /* __MS_CONSOLE_H__ */
