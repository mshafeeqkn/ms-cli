#include <stdio.h>
#include <execinfo.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "ms_log.h"

static void full_write(int fd, const char *buf, size_t len)
{
    while (len > 0) {
        ssize_t ret = write(fd, buf, len);

        if ((ret == -1) && (errno != EINTR))
            break;

        buf += (size_t) ret;
        len -= (size_t) ret;
    }
}

void crash_handler(int sig) {
    void *array[10];
    size_t bt_size;
    char **bt_syms;
    int i;
    char dev[128] = {0};
    int dev_fd = STDERR_FILENO;

    // get void*'s for all entries on the stack
    bt_size = backtrace(array, 10);

    // print out all the frames to stderr
    ms_err("Error: signal %d:\n", sig);
    bt_syms = backtrace_symbols(array, bt_size);

    FILE *fp = fopen("/tmp/.log_file", "r");
    if(fp != NULL) {
        fgets(dev, sizeof(dev), fp);
        dev[strlen(dev)-1] = 0;
        dev_fd = open(dev, O_WRONLY, S_IWUSR);
        fclose(fp);
    }

    for (i = 1; i < bt_size; i++) {
        size_t len = strlen(bt_syms[i]);
        full_write(dev_fd, bt_syms[i], len);
        full_write(dev_fd, "\n", 1);
    }

    close(dev_fd);

    exit(1);
}
