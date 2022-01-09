#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ms_entry.h"
#include "ms_log.h"
#include "ms_cmd.h"

ms_status_t test_regsiter_after() {
    ms_status_t ret;
    int i;
    ms_entry_t *head;
    ms_entry_t *copy, *entry;
    char buff[128] = {0};

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

void *test_function_enable(void* args) {
    return NULL;
}


void test_ms_cmd() {
    ms_cmd_t *cmd0 = ms_cmd_create(0x1, "enable", "Enable privileged commands",
                                  "<0-1>", "This is the dummy arg help",
                                  test_function_enable);

    ms_cmd_t *cmd1 = ms_cmd_create(0x2, "show  ", "Show running system informations",
                                   "<0-1>", "This is the dummy arg help",
                                   test_function_enable);
    ms_cmd_t *cmd11 = ms_cmd_create(0x20, "ip    ", "Show IP infromations",
                                    "<0-1>", "This is the dummy arg help",
                                    test_function_enable);
    ms_cmd_t *cmd12 = ms_cmd_create(0x21, "vlan  ", "Show VLAN infromations",
                                    "<0-1>", "This is the dummy arg help",
                                    test_function_enable);
    ms_cmd_t *cmd120 = ms_cmd_create(0x210, "brief", "Show VLAN infromations in brief",
                                     "<0-1>", "This is the dummy arg help",
                                     test_function_enable);
    ms_cmd_t *cmd121 = ms_cmd_create(0x211, "id   ", "Show VLAN infromation by VLAN ID",
                                     "<0-1>", "This is the dummy arg help",
                                     test_function_enable);
    ms_cmd_t *cmd122 = ms_cmd_create(0x212, "name  ", "Show VLAN infromation by name",
                                     "<0-1>", "This is the dummy arg help",
                                     test_function_enable);
    ms_cmd_t *cmd13 = ms_cmd_create(0x22, "mac  ", "Show MAC infromation",
                                    "<0-1>", "This is the dummy arg help",
                                    test_function_enable);

    ms_cmd_t *cmd2 = ms_cmd_create(0x3, "disable", "Turn off privileged commands",
                                   "<0-1>", "This is the dummy arg help",
                                   test_function_enable);

    ms_cmd_t *cmd3 = ms_cmd_create(0x4, "connect", "Open terminal connection",
                                   "<0-1>", "This is the dummy arg help",
                                   test_function_enable);

    ms_cmd_hook_at_end(cmd0, cmd1); //show
      ms_cmd_hook_as_child(cmd1, cmd11);    // show ip
      ms_cmd_hook_as_child(cmd1, cmd12);    // show vlan
        ms_cmd_hook_as_child(cmd12, cmd120);    // show vlan breif
        ms_cmd_hook_as_child(cmd12, cmd121);    // show vlan id
        ms_cmd_hook_as_child(cmd12, cmd122);    // show vlan name
      ms_cmd_hook_as_child(cmd1, cmd13);    // show mac
    ms_cmd_hook_at_end(cmd0, cmd2); //disable
    ms_cmd_hook_at_end(cmd0, cmd3); // connect

    ms_cmd_dbg_print(cmd0);
    ms_cmd_dbg_print(cmd1);
    ms_cmd_dbg_print(cmd11);
    ms_cmd_dbg_print(cmd12);
    ms_cmd_dbg_print(cmd120);
    ms_cmd_dbg_print(cmd121);
    ms_cmd_dbg_print(cmd122);
    ms_cmd_dbg_print(cmd13);
    ms_cmd_dbg_print(cmd2);
    ms_cmd_dbg_print(cmd3);
}

int main(int argc, char *argv[]) {
    ms_log(log_dbg, "\033[2J");
#if 0
    ms_status_t ret;
    ret = test_regsiter_after();
    if(ret != ms_st_ok)
        ms_log(log_dbg, "============The test test_regsiter_after failed============");
#endif

    test_ms_cmd();

    return 0;
}
