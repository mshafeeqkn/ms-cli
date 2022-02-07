#include <execinfo.h>
#include <unistd.h>
#include <stdlib.h>

#include "ms_log.h"

void crash_handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    ms_err("Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}
