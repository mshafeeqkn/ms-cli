#include<stdio.h>
#include <stdarg.h>


#include "ms_log.h"

static const char *term_file = "/dev/pts/2";

log_level_t log_level = log_dbg;

void _ms_log(log_level_t level, char *file, int line,  const char* format, ...) {
    FILE *fp;
    va_list args;
    char f[128] = {0};

    if(level > log_level)
        return;

    fp = fopen(term_file, "w");
    sprintf(f, "%s:%d ", file, line);
    fprintf(fp, "%-18s|", f);
    va_start (args, format);
    vfprintf (fp, format, args);
    fprintf(fp, "\n");
    va_end (args);
    fclose(fp);
}
