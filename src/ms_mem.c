#include <stdlib.h>

#include "ms_log.h"

void *_ms_alloc(char *file, int line, size_t size) {
    void *ret = malloc(size);
    memset(ret, 0, size);
    ms_log(log_mem, "%s:%d %d bytes ++memory allocated at: %x", file, line, size, ret);
    return ret;
}

void _ms_free(char *file, int line, void *ptr) {
    ms_log(log_mem, "%s:%d --deallocated memory allocated at: %x", file, line, ptr);
    free(ptr);
}
