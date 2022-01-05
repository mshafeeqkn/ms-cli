#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "cmd_map.h"

void print_entry(cmd_entry_t *entry) {
    printf("cmd: %s\t-\t%s, callback: %p, child: %p\n", entry->cmd, entry->help, entry->func, entry->child);
}

int cmd_disabled_enable(void *data) {
    printf("invoking: %s\n", __func__);
    return 0;
}

int cmd_show_dynamic_mac_table(void *data) {
    printf("invoking: %s\n", __func__);
    return 0;
}

int cmd_show_static_mac_table(void *data) {
    printf("invoking: %s\n", __func__);
    return 0;
}

int cmd_disabled_disable(void *data) {
    printf("invoking: %s\n", __func__);
    return 0;
}

int cmd_disabled_show(void *data) {
    printf("invoking: %s\n", __func__);
    return 0;
}

int cmd_show_arp_table(void *data) {
    printf("invoking: %s\n", __func__);
    return 0;
}

int cmd_show_mac_table(void *data) {
    printf("invoking: %s\n", __func__);
    return 0;
}

int cmd_show_vlan_table(void *data) {
    printf("invoking: %s\n", __func__);
    return 0;
}

cmd_entry_t last_cmd_map[] = {
    {
        "<cr>",
        "",
        NULL,
        NULL
    }
};

cmd_entry_t mac_table_types[] = {
    {
        "dynamic",
        "Dynamic entry type",
        cmd_show_dynamic_mac_table,
        last_cmd_map
    },
    {
        "static",
        "Static entry type",
        cmd_show_static_mac_table,
        last_cmd_map
    },
    {
        "<cr>",
        "",
        NULL,
        NULL
    }
};

cmd_entry_t disable_show_cmd_map[] = {
    {
        "arp",
        "ARP table",
        cmd_show_arp_table,
        last_cmd_map
    },
    {
        "mac-address-table",
        "MAC forwarding table",
        cmd_show_mac_table,
        mac_table_types
    },
    {
        "vlan",
        "VTP VLAN table",
        cmd_show_vlan_table,
        last_cmd_map
    },
};

cmd_entry_t disable_cmd_map[] = {
    {
        "disable",
        "Trun off privileged commands",
        cmd_disabled_disable,
        last_cmd_map
    },
    {
        "enable",
        "Trun on privileged commands",
        cmd_disabled_enable,
        last_cmd_map
    },
    {
        "show",
        "Show running system information",
        cmd_disabled_show,
        disable_show_cmd_map
    },
    {
        NULL, NULL, NULL, NULL
    }
};

extern void print_help(cmd_entry_t cmd_map[]) {
    int i = 0;

    printf("\n");
    do {
        if(!cmd_map[i].cmd)
            break;
        printf("%s\t-\t%s\n", cmd_map[i].cmd, cmd_map[i].help);
    } while(++i);
}


#if 0
int main(int argc, char *argv[]) {
    print_entry(&disable_cmd_map[0]);
}
#endif
