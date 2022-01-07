#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ms_entry.h"
#include "ms_log.h"

ms_status_t test_regsiter_after() {
    ms_status_t ret;
    int i;
    ms_entry_t *head;
    ms_entry_t *copy, *entry;
    char buff[128] = {0};

    ms_log(log_dbg, "\033[2J");
    sprintf(buff, "test_0");
    head = ms_entry_create(buff);
    if(head == NULL)
        return -ms_st_mem_err;
	ms_log(log_dbg, "Head create done");

    ret = ms_entry_set_prefix(head, ">");
    if(ret != ms_st_ok)
        return ret;
	ms_log(log_dbg, "Prefix set done");

    for(i = 0; i < 4; i++) {
        sprintf(buff, "test_%d", i+1);
        entry = ms_entry_create(buff);
        if(entry == NULL)
            return -ms_st_mem_err;
        if(i == 2)
            copy = entry;

        ret = ms_entry_set_prefix(entry, ">");
        if(ret != ms_st_ok)
            return ret;

        ret = ms_entry_hook_after(head, entry);
        if(ret != ms_st_ok)
            return ret;
    }
	ms_log(log_dbg, "Node add done");

    entry = ms_entry_create(EMPTY_STR);
    if(entry == NULL)
        return -ms_st_mem_err;
    ms_log(log_dbg, "Creating empty done");

    ret = ms_entry_set_prefix(head, ">");
    if(ret != ms_st_ok)
        return ret;
    ms_log(log_dbg, "Prefix set done");

    ret = ms_entry_hook_after(head, entry);
    if(ret != ms_st_ok)
        return ret;

    ms_log(log_dbg, "");

    ret = ms_entry_copy_to_list_end(head, copy);
    if(ret != ms_st_ok)
        return ret;

    ms_log(log_dbg, "=========Forward========");
    ms_dbg_print_history(head, 1);
    return ms_st_ok;
}


int main(int argc, char *argv[]) {
    ms_status_t ret;
    ret = test_regsiter_after();
    if(ret != ms_st_ok)
        ms_log(log_dbg, "============The test test_regsiter_after failed============");

    return 0;
}
