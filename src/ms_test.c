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


ms_cmd_t* pre_test_ms_cmd() {
    ms_cmd_t *cmd01 = ms_cmd_create(0x01, "enable", "Enable privileged commands",
                                  "<0-1>", "This is the dummy arg help",
                                  test_function_enable);

    ms_cmd_t *cmd02 = ms_cmd_create(0x02, "show  ", "Show running system informations",
                                   "<0-1>", "This is the dummy arg help",
                                   test_function_enable);
    ms_cmd_t *cmd0201 = ms_cmd_create(0x01, "ip    ", "Show IP infromations",
                                    "<0-1>", "This is the dummy arg help",
                                    test_function_enable);
    ms_cmd_t *cmd0202 = ms_cmd_create(0x02, "vlan  ", "Show VLAN infromations",
                                    "<0-1>", "This is the dummy arg help",
                                    test_function_enable);
    ms_cmd_t *cmd020201 = ms_cmd_create(0x01, "brief", "Show VLAN infromations in brief",
                                     "<0-1>", "This is the dummy arg help",
                                     test_function_enable);
    ms_cmd_t *cmd020202 = ms_cmd_create(0x02, "id   ", "Show VLAN infromation by VLAN ID",
                                     "<0-1>", "This is the dummy arg help",
                                     test_function_enable);
    ms_cmd_t *cmd020203 = ms_cmd_create(0x03, "name  ", "Show VLAN infromation by name",
                                     "<0-1>", "This is the dummy arg help",
                                     test_function_enable);
    ms_cmd_t *cmd0203 = ms_cmd_create(0x03, "mac  ", "Show MAC infromation",
                                    "<0-1>", "This is the dummy arg help",
                                    test_function_enable);

    ms_cmd_t *cmd03 = ms_cmd_create(0x03, "disable", "Turn off privileged commands",
                                   "<0-1>", "This is the dummy arg help",
                                   test_function_enable);

    ms_cmd_t *cmd04 = ms_cmd_create(0x04, "connect", "Open terminal connection",
                                   "<0-1>", "This is the dummy arg help",
                                   test_function_enable);

    ms_cmd_hook_at_end(cmd01, cmd02); //show
      ms_cmd_hook_as_child(cmd02, cmd0201);    // show ip
      ms_cmd_hook_as_child(cmd02, cmd0202);    // show vlan
        ms_cmd_hook_as_child(cmd0202, cmd020201);    // show vlan breif
        ms_cmd_hook_as_child(cmd0202, cmd020202);    // show vlan id
        ms_cmd_hook_as_child(cmd0202, cmd020203);    // show vlan name
      ms_cmd_hook_as_child(cmd02, cmd0203);    // show mac
    ms_cmd_hook_at_end(cmd01, cmd03); //disable
    ms_cmd_hook_at_end(cmd01, cmd04); // connect

    ms_cmd_dbg_print(cmd01);
    ms_cmd_dbg_print(cmd02);
    ms_cmd_dbg_print(cmd0201);
    ms_cmd_dbg_print(cmd0202);
    ms_cmd_dbg_print(cmd020201);
    ms_cmd_dbg_print(cmd020202);
    ms_cmd_dbg_print(cmd020203);
    ms_cmd_dbg_print(cmd0203);
    ms_cmd_dbg_print(cmd03);
    ms_cmd_dbg_print(cmd04);

    return cmd01;
}

static unsigned char cmd_path[MAX_SUB_COMMANDS] = {0};

int main(int argc, char *argv[]) {
    ms_log(log_dbg, "\033[2J");
#if 0
    ms_status_t ret;
    ret = test_regsiter_after();
    if(ret != ms_st_ok)
        ms_log(log_dbg, "============The test test_regsiter_after failed============");
#endif

    ms_log(log_dbg, "int : %d, long long int: %d", sizeof(int), sizeof(unsigned long long));
    cmd_path[0] = 0x02;
    cmd_path[1] = 0x02;
    ms_cmd_t *tree = pre_test_ms_cmd();
    ms_cmd_show_cmd_help(tree, cmd_path, sizeof(cmd_path));
    return 0;
}
