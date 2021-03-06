#include <stdio.h>
#include <stdlib.h>

#include "ms_entry.h"
#include "ms_defines.h"
#include "ms_log.h"
#include "ms_mem.h"
#include "ms_utils.h"

extern ms_entry_t* ms_entry_create(char *str) {
    int len = strlen(str);

    ms_entry_t *ret = ms_alloc(sizeof(ms_entry_t));
    if(!ret)
        return NULL;

    ret->str = ms_alloc(len + 1);
    if(!ret->str) {
        ms_free(ret);
        return NULL;
    }

    strncpy(ret->str, str, len);
    ret->str[len] = 0;

    ret->len = len;
    ret->cursor = len;
    ret->next = NULL;
    ret->prev = NULL;

    ms_log(log_dbg, "Created new entry node: 0x%x", addr(ret));
    return ret;
}

void ms_entry_free(ms_entry_t *entry) {
    ms_free(entry->str);
    ms_free(entry->prefix);
    ms_free(entry);
}

void _ms_dbg_print_entry(char *pref, ms_entry_t *entry) {
    ms_log(log_dbg, "%s - 0x%06x<(this:0x%06x)>0x%06x %s %-10s\tlen: %d, pos: %d",
        pref, addr(entry->prev), addr(entry), addr(entry->next),
        entry->prefix, entry->str, entry->len, entry->cursor);
}

ms_status_t ms_entry_set_string(ms_entry_t *entry, char *command) {
    int cmd_len;
    char *tmp;
    char *cmd;
    int len, buff_len;

    if(!entry || !command)
        return -ms_st_null_arg;

    cmd_len = strlen(command);
    ms_entry_get_last_command(entry, &cmd, &len);
    buff_len = entry->len - len + cmd_len;
    tmp = ms_alloc(buff_len + 1);
    snprintf(tmp, buff_len + 1, "%s%s", entry->str, command+len);
    ms_free(entry->str);
    entry->str = tmp;
    entry->len = buff_len;
    entry->cursor = buff_len;
    return ms_st_ok;
}

ms_status_t ms_entry_set_prefix(ms_entry_t *entry, char *prefix) {
    int pref_len;

    if(!entry || !prefix)
        return -ms_st_null_arg;

    pref_len = strlen(prefix);
    // TODO: Free existing mem
    entry->prefix = ms_alloc(pref_len + 1);
    if(!entry->prefix)
        return -ms_st_mem_err;

    strncpy(entry->prefix, prefix, pref_len);
    entry->prefix[pref_len] = 0;
    return ms_st_ok;
}

ms_status_t ms_entry_set_cursor(ms_entry_t *entry, int cursor) {
    if(!entry)
        return -ms_st_null_arg;

    if(cursor < -1 || cursor > entry->len)
        return -ms_st_inval_arg;

    entry->cursor = cursor;

    return ms_st_ok;
}

ms_status_t ms_entry_cursor_fw(ms_entry_t *entry) {
    if(entry->cursor == entry->len)
        return -ms_st_limit_exhaust;

    entry->cursor++;
    return ms_st_ok;
}

ms_status_t ms_entry_cursor_bw(ms_entry_t *entry) {
    if(entry->cursor == 0)
        return -ms_st_limit_exhaust;

    entry->cursor--;
    return ms_st_ok;
}

ms_status_t ms_entry_insert_char_at(ms_entry_t *entry, int pos, char ch) {
    char tmp[MAX_COMMAND_LEN] = {0};
    ms_status_t ret;

    if(pos < 0 || pos > entry->len)
        return -ms_st_inval_arg;

    strncpy(tmp, entry->str, pos);

    tmp[pos] = ch;
    strcpy(tmp + pos + 1, entry->str + pos);

    entry->len++;
    ret = ms_entry_set_cursor(entry, pos + 1);
    if(ret != ms_st_ok)
        return ret;

    ms_free(entry->str);
    entry->str = ms_alloc(entry->len);
    if(!entry->str)
        return -ms_st_mem_err;

    strncpy(entry->str, tmp, entry->len);
    entry->str[entry->len] = 0;

    return ms_st_ok;
}

ms_status_t ms_entry_insert_char(ms_entry_t *entry, char ch) {
    return ms_entry_insert_char_at(entry, entry->cursor, ch);
}

ms_status_t ms_entry_remove_char(ms_entry_t *entry) {
    if(entry->cursor == 0 || entry->cursor > entry->len)
        return -ms_st_fail;

    if(entry->len != entry->cursor) {
        strcpy(entry->str + entry->cursor - 1, entry->str + entry->cursor);
    }

    entry->cursor--;
    entry->len--;
    entry->str[entry->len] = 0;
    return ms_st_ok;
}

void ms_print_entry(ms_entry_t *entry) {
    char buff[MAX_COMMAND_LEN] = {0};
    snprintf(buff, MAX_COMMAND_LEN, "%s %s", entry->prefix, entry->str);
    printf("\033[2K");
    printf("\r%s", buff);
    if(entry->len != entry->cursor)
        printf("\033[%dD", entry->len - entry->cursor);
    fflush(stdout);
}

ms_status_t ms_entry_hook_after(ms_entry_t *head, ms_entry_t *entry) {
    if(head == NULL || entry == NULL) {
        return -ms_st_null_arg;
    }

    if(entry->next) {
        entry->next->prev = entry->prev;
        entry->prev->next = entry->next;
    }
    while(head->next)
        head = head->next;

    head->next = entry;
    entry->prev = head;

    return ms_st_ok;
}

ms_status_t ms_entry_replace_last_word(ms_entry_t *entry, char *cmd) {
    int i = entry->len;
    char *tmp = entry->str;
    int cmd_len = strlen(cmd);

    while(entry->str[--i] != ' ' && i != 0);
    entry->str[i] = 0;
    entry->str = ms_alloc(i + cmd_len);
    if(IS_EMPTY_STR(tmp)) {
        strncpy(entry->str, cmd, cmd_len);
        entry->len = cmd_len;
        entry->cursor = cmd_len;
    } else {
        snprintf(entry->str, i + cmd_len + 2, "%s %s", tmp, cmd);   // +1 for space
                                                                    // +1 for str end NULL
        entry->len = i + cmd_len + 1;
        entry->cursor = i + cmd_len + 1;
    }
    ms_free(tmp);

    return ms_st_ok;
}

ms_status_t ms_entry_copy_data(ms_entry_t *dst, ms_entry_t *src) {
    if(src == NULL || dst == NULL)
        return -ms_st_null_arg;

    int pref_len = strlen(src->prefix);

    if(dst->str)
        ms_free(dst->str);
    dst->str = ms_alloc(src->len + 1);
    if(dst->str == NULL)
        return -ms_st_mem_err;

    if(dst->prefix)
        ms_free(dst->prefix);
    dst->prefix = ms_alloc(pref_len + 1);
    if(dst->prefix == NULL)
        return -ms_st_mem_err;

    snprintf(dst->str, src->len, "%s", src->str);
    snprintf(dst->prefix, pref_len, "%s", src->prefix);
    dst->len = src->len;
    dst->cursor = src->len;

    return ms_st_ok;
}

ms_status_t ms_entry_copy_to_list_end(ms_entry_t *head, ms_entry_t *node) {
    ms_status_t ret;

    if(head == NULL || node == NULL)
        return -ms_st_null_arg;

    ms_entry_t *tmp = ms_entry_create(node->str);
    if(tmp == NULL) {
        ms_log(log_dbg, "Error: Failed to create new node");
        return -ms_st_fail;
    }

    ret = ms_entry_set_prefix(tmp, node->prefix);
    if(ret != ms_st_ok)
        return ret;

    memset(tmp, 0, sizeof(ms_entry_t));
    ms_entry_t *tail = head;
    while(tail->next)
        tail = tail->next;

    if(tail->len == 0) {
        // If last node doesn't have any content, remove it.
        tail = tail->prev;
        ms_entry_free(tail->next);
    }

    tail->next = tmp;
    tmp->prev = tail;
    ret = ms_entry_copy_data(tmp, node);
    return ret;
}

ms_status_t ms_entry_get_last_command(ms_entry_t *entry, char **cmd, int *len) {
    if(entry->str[entry->len-1] == ' ') {
        *cmd = NULL;
        *len = 0;
    } else {
        *cmd = strrchr(entry->str, ' ');
        if(*cmd == NULL) {
            *cmd = entry->str;
            *len = strlen(entry->str);
        } else {
            (*cmd)++;
            *len = strlen(*cmd);
        }
    }
    return ms_st_ok;
}

void ms_dbg_print_history(ms_entry_t *entry, int fw) {
    while(entry) {
         ms_dbg_print_entry(entry);
        if(fw)
            entry = entry->next;
        else
            entry = entry->prev;
    }
}
